#include <signal.h>

/* public headers */
#include "../include/liberty.h"

/* debugging macros */
#ifdef LIBERTY_DEBUG
#define LOG(...) fprintf(stderr, __VA_ARGS__);
#else
#define LOG(...) do { } while(0);
#endif

#define SDL_ERR() fprintf(stderr, "ERROR: %s\n", SDL_GetError());

/* private globals */
LibertyConfig Config;
SDL_Window   *Window;
SDL_Renderer *Renderer;

/* private source */
#include "events.c"
#include "window.c"

/* public source */
#include "callback.c"
#include "font.c"
#include "config.c"

/* prototypes */
static void cleanup(int signal);
static void update(void);
static void draw(void);
static void handle_signal(LibertySignal signal);

/* implementation */
static void cleanup(int signal)
{
LOG("calling liberty_callback_cleanup\n");
	liberty_callback_cleanup();
	destroy_window();
LOG("quitting SDL...\n");
	SDL_Quit();
LOG("SDL quit\n");
LOG("baiii!\n");
	exit(signal);
}

static void update(void)
{
	static uint64_t lasttime, nowtime;
	static float frametime;
	lasttime = nowtime;
	nowtime = SDL_GetPerformanceCounter();
	float deltatime = (nowtime - lasttime)*1000 / (float)SDL_GetPerformanceFrequency();
// LOG("updating: %ldms, %.6fΔ...\n", SDL_GetTicks64(), deltatime);
	handle_signal(liberty_callback_update(deltatime));
	if (Config.framerate < 1.0f) /* unlimited framerate */
		draw();
	else
	{
		frametime += deltatime;
		float targetframetime = 1.0f / Config.framerate; /* TODO: doesn't need to be calculated every tick */
		if (frametime > targetframetime)
		{
			frametime -= targetframetime;
			draw();
		}
	}
}

static void draw(void)
{
	liberty_callback_draw(Renderer);
}

static void handle_signal(LibertySignal signal)
{
	switch (signal)
	{
		case LIBERTY_SIGNAL_OK: break;
		case LIBERTY_SIGNAL_TERM: LOG("signal TERM\n"); cleanup(0); break;
		case LIBERTY_SIGNAL_UPDATE: LOG("signal UPDATE\n"); update(); break;
	}
}

int main(int argc, char *argv[])
{
	SDL_Event event;
LOG("welcome to liberty!\n");

	Config = liberty_default_config;

LOG("initializing SDL\n");
	SDL_Init(SDL_INIT_EVERYTHING);

LOG("initializing SDL user events\n")
	register_sdl_user_events();

LOG("trapping signals\n");
	signal(SIGINT , cleanup);
	signal(SIGTERM, cleanup);

LOG("initializing Window\n");
	create_window();

LOG("calling liberty_callback_init\n");
	liberty_callback_init();

LOG("starting the main game loop\n");
	/* main game loop */
	while (1)
	{
		/* event */
		while (SDL_PollEvent(&event))
		{
			/* LIBERTY_EVENT isn't known at compile time */
			if (event.type == LIBERTY_EVENT)
			{
				switch (event.user.code)
				{
					case LIBERTY_EVENT_CONFIG:
LOG("applying new Config\n");
						memcpy(&Config, event.user.data1, sizeof(LibertyConfig));
						free(event.user.data1);
						reapply_config();
						break;
				}
			} else
				switch (event.type)
				{
					case SDL_EVENT_QUIT: /* fall through */
					case SDL_EVENT_WINDOW_CLOSE_REQUESTED: cleanup(0);
					case SDL_EVENT_WINDOW_SHOWN: /* fall through */
					case SDL_EVENT_WINDOW_EXPOSED: draw(); break;
					// case SDL_EVENT_WINDOW_RESIZED: reapply_config(); break; /* reapply the window size if the window get's resized */
					default:
						handle_signal(liberty_callback_event(event));
						break;
				}
		}

		/* update */
		update();

	}
	return 0; /* this return should be unreachable */
}

#include <stdlib.h>
#include <signal.h>
#include <SDL.h>
#include "../include/liberty.h"

/* prototypes */
static void cleanup(int signal);
static void update(void);
static void handle_signal(LibertySignal signal);

/* debugging macros */
#ifdef LIBERTY_DEBUG
#define LOG(...) fprintf(stderr, __VA_ARGS__);
#else
#define LOG(...) do { } while(0);
#endif

/* implementation */
static void cleanup(int signal)
{
LOG("calling liberty_callback_cleanup\n");
	liberty_callback_cleanup();
LOG("quitting SDL...\n");
	SDL_Quit();
LOG("SDL quit\n");
LOG("baiii!\n");
	exit(signal);
}

static void update(void)
{
	static uint64_t lasttime, nowtime;
	lasttime = nowtime;
	nowtime = SDL_GetPerformanceCounter();
	float deltatime = (nowtime - lasttime)*1000 / (float)SDL_GetPerformanceFrequency();
// LOG("updating: %ldms, %.6fΔ...\n", SDL_GetTicks64(), deltatime);
	handle_signal(liberty_callback_update(deltatime));
}

static void handle_signal(LibertySignal signal)
{
	switch (signal)
	{
		case LIBERTY_SIGNAL_OK: break;
		case LIBERTY_SIGNAL_TERM: LOG("signal TERM\n"); cleanup(0); break;
		case LIBERTY_SIGNAL_DRAW: LOG("signal DRAW\n"); liberty_callback_draw(); break;
		case LIBERTY_SIGNAL_UPDATE: LOG("signal UPDATE\n"); update(); break;
	}
}

int main(int argc, char *argv[])
{
	SDL_Event event;
LOG("welcome to liberty!\n");

LOG("initializing SDL...\n");
	SDL_Init(SDL_INIT_EVERYTHING);
LOG("SDL initialized\n");

LOG("trapping signals\n");
	signal(SIGINT , cleanup);
	signal(SIGTERM, cleanup);

LOG("calling liberty_callback_init\n");
liberty_callback_init();

LOG("starting the main game loop\n");
	/* main game loop */
	while (1)
	{
		/* event */
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_WINDOWEVENT:
					switch (event.window.event)
					{
						case SDL_WINDOWEVENT_CLOSE: cleanup(0);
					} break;
				case SDL_KEYDOWN: /* fall through */
				case SDL_KEYUP: {
						LibertyEvent le;
						le.type = event.key.repeat ? LIBERTY_EVENT_KEYREPEAT : (event.type == SDL_KEYUP ? LIBERTY_EVENT_KEYRELEASE : LIBERTY_EVENT_KEYPRESS);
						le.key.keycode = event.key.keysym.sym;
LOG("key %s %s\n", SDL_GetKeyName(le.key.keycode), (le.type == LIBERTY_EVENT_KEYREPEAT) ? "repeated" : ((le.type == LIBERTY_EVENT_KEYRELEASE) ? "released" : "pressed"));
						handle_signal(liberty_callback_event(le));
					} break;
			}
		}

		/* update */
		update();

	}
	return 0; /* this return should be unreachable */
}

#include "window.c"
#include "callback.c"
#include "draw.c"

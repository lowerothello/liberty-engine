#include <stdlib.h>
#include <signal.h>
#include <SDL.h>
#include "../include/liberty.h"

/* define a lil' logging macro */
#ifdef LIBERTY_DEBUG
#define LOG(...) fprintf(stderr, __VA_ARGS__);
#else
#define LOG(...) do { } while(0);
#endif

static void cleanup(int signal)
{
LOG("quitting SDL...\n");
	SDL_Quit();
LOG("SDL quit\n");
LOG("baiii!\n");
	exit(signal);
}

int main(int argc, char *argv[])
{
	SDL_Event event;
	uint64_t lasttime = 0, nowtime = SDL_GetPerformanceCounter();
	float deltatime;
LOG("welcome to liberty!\n");

LOG("initializing SDL...\n");
	SDL_Init(SDL_INIT_EVERYTHING);
LOG("SDL initialized\n");

LOG("trapping signals\n");
	signal(SIGINT , cleanup);
	signal(SIGTERM, cleanup);

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
				case SDL_KEYDOWN:
					break;
				case SDL_KEYUP:
					break;
			}
		}

		/* update */
		lasttime = nowtime;
		nowtime = SDL_GetPerformanceCounter();
		deltatime = (nowtime - lasttime)*1000 / (float)SDL_GetPerformanceFrequency();
LOG("updating: %ldms, %.6fΔ...\n", SDL_GetTicks64(), deltatime);
		switch (liberty_callback_update(deltatime))
		{
			case LIBERTY_SIGNAL_OK:
LOG("update OK\n");
				break;
			case LIBERTY_SIGNAL_TERM:
LOG("update TERM\n");
				cleanup(0);
				break;
			case LIBERTY_SIGNAL_DRAW:
LOG("update DRAW\n");
LOG("drawing frame...\n");
				/* TODO: draw */
LOG("finished drawing\n");
				break;
		}

	}
	return 0; /* this return should be unreachable */
}

#include "callback.c"
#include "draw.c"
#include "input.c"

#include <stdlib.h>
#include <signal.h>
#include <SDL.h>
#include "../include/liberty.h"

Liberty *LIBERTY;

static void privliberty_cleanup(int signal)
{
	if (LIBERTY)
	{
		SDL_DestroyWindow(LIBERTY->window);
		SDL_VideoQuit();
		SDL_Quit();
		free(LIBERTY);
	}
	exit(signal);
}

int main(int argc, char *argv[])
{
	LIBERTY = liberty_callback_init();
	SDL_Event event;

	/* trap signals */
	signal(SIGINT , privliberty_cleanup);
	signal(SIGTERM, privliberty_cleanup);

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
						case SDL_WINDOWEVENT_CLOSE: privliberty_cleanup(0);
					} break;
				case SDL_KEYDOWN:
					break;
				case SDL_KEYUP:
					break;
			}
		}

		/* update */
		switch (liberty_callback_update(LIBERTY, SDL_GetTicks64()))
		{
			case LIBERTY_SIGNAL_OK: break;
			case LIBERTY_SIGNAL_TERM: privliberty_cleanup(0);
		}

		/* TODO: draw */
	}
	return 0;
}

#include "init.c"
#include "callback.c"
#include "draw/draw.c"

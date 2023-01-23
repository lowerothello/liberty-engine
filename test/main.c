#include "../include/liberty.h"

// void liberty_callback_init(void)
// {
// 	window = liberty_create_window(
// 			LIBERTY_DEFAULT_TITLE,
// 			LIBERTY_DEFAULT_SCALE,
// 			LIBERTY_DEFAULT_WIDTH,
// 			LIBERTY_DEFAULT_HEIGHT,
// 			LIBERTY_DEFAULT_FRAMERATE);
// }
//
// void liberty_callback_cleanup(void)
// {
// 	liberty_destroy_window(window);
// }


LibertySignal liberty_callback_event(SDL_Event event)
{
	LibertyConfig config = liberty_get_config();
	switch (event.type)
	{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
				case 'a':
					config.scale++;
					liberty_set_config(config);
					break;
				case 'x':
					config.scale--;
					liberty_set_config(config);
					break;
			}
			break;
		case SDL_KEYUP:
			break;
	}
	return LIBERTY_SIGNAL_OK;
}

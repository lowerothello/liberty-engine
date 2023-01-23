#include "../include/liberty.h"

#define LIBERTY_DEFAULT_TITLE "Liberty"
#define LIBERTY_DEFAULT_SCALE 1
#define LIBERTY_DEFAULT_WIDTH 640
#define LIBERTY_DEFAULT_HEIGHT 480
#define LIBERTY_DEFAULT_FRAMERATE 0

LibertyWindow *window;

void liberty_callback_init(void)
{
	window = liberty_create_window(
			LIBERTY_DEFAULT_TITLE,
			LIBERTY_DEFAULT_SCALE,
			LIBERTY_DEFAULT_WIDTH,
			LIBERTY_DEFAULT_HEIGHT,
			LIBERTY_DEFAULT_FRAMERATE);
}

void liberty_callback_cleanup(void)
{
	liberty_destroy_window(window);
}

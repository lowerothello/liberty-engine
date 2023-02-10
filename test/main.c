#include "../include/liberty.h"

#define WIDTH 320
#define HEIGHT 180

LibertyConfig liberty_default_config =
{
	.title = "Liberty",
	.width = WIDTH,
	.height = HEIGHT,
	.scale = 3,
	.fullscreen = 0,
	.vsync = 0,
	.framerate = 0.0f,
};

LibertyFont  *font;
LibertyLayer *layer;
void liberty_callback_init(void)
{
	font = liberty_new_font_from_file("Untitled1-6.bdf");
	layer = liberty_new_layer();
// liberty_free_font(font);
// exit(0);
}

void liberty_callback_cleanup(void)
{
	liberty_free_font(font);
	liberty_free_layer(layer);
}

void liberty_callback_draw(double frametime)
{
	liberty_set_draw_layer(layer);
	liberty_set_colour((LibertyColour){0x00, 0x00, 0x00, 0xff});
	liberty_draw_clear();

	liberty_set_colour((LibertyColour){0xff, 0xff, 0xff, 0xff});

	char buffer[32];
	snprintf(buffer, 32, "%f", 1.0 / frametime);
	liberty_draw_font_string(font, (SDL_Point){WIDTH - 40, 1}, buffer);

	LIBERTY_DRAW_FONT_STRING_CENTRE(font, ((SDL_Rect){0, 0, WIDTH, HEIGHT}), "should be in the centre of the screen");

	liberty_set_draw_layer(NULL);
	liberty_draw_layer(layer, (SDL_Point){-1,  0}, (LibertyColour){0xff, 0x00, 0x00, 0xff});
	liberty_draw_layer(layer, (SDL_Point){ 0,  0}, (LibertyColour){0x00, 0xff, 0x00, 0xff});
	liberty_draw_layer(layer, (SDL_Point){ 1,  0}, (LibertyColour){0x00, 0x00, 0xff, 0xff});
}

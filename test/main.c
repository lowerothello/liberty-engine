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

LibertyFont *font;
void liberty_callback_init(void)
{
	font = liberty_new_font_from_file("Untitled1-6.bdf");
// liberty_free_font(font);
// exit(0);
}

void liberty_callback_cleanup(void)
{
	liberty_free_font(font);
}

double clock = 0;
LibertySignal liberty_callback_update(double deltatime)
{
	clock += deltatime;
	while (clock >= 10)
		clock -= 10;
	return LIBERTY_SIGNAL_OK;
}

void liberty_callback_draw(double frametime)
{
	// SDL_SetRenderDrawColor(renderer, 0xff,0xff,0x00,0x00);
	// SDL_RenderClear(renderer);
	// SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0xff);
	// liberty_draw_font_string(font, "TESTING STRING: THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", (SDL_Point){5, 10});
	// liberty_draw_font_string(font, "line 1\nline 2", (SDL_Point){20, 20});
	// SDL_RenderPresent(renderer);
	liberty_set_colour((LibertyColour){0x00, 0x00, 0x00, 0xff});
	liberty_draw_clear();

	liberty_set_colour((LibertyColour){0xff, 0xff, 0xff, 0xff});
	liberty_draw_rect(0, (SDL_Rect){0, 0, WIDTH, HEIGHT});

	char buffer[32];
	snprintf(buffer, 32, "%f", 1.0 / frametime);
	liberty_draw_font_string(font, (SDL_Point){WIDTH - 40, 1}, buffer);

	snprintf(buffer, 32, "%f", clock);
	liberty_draw_font_string(font, (SDL_Point){WIDTH>>1, HEIGHT>>1}, buffer);

	liberty_draw_font_string_outline(font, (SDL_Point){20, 60}, "test string");
}

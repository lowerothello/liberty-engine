#include "../include/liberty.h"

LibertyFont *font;
void liberty_callback_init(void)
{
	font = liberty_new_font_from_file("test.bdf");
// liberty_free_font(font);
// exit(0);
}

void liberty_callback_cleanup(void)
{
	liberty_free_font(font);
}

void liberty_callback_draw(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0xff,0xff,0x00,0x00);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0xff);
	liberty_font_string(font, "testing string: the quick brown fox jumps over the lazy dog", (SDL_Point){5, 10});
	SDL_RenderPresent(renderer);
}

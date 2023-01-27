#include "../include/liberty.h"

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

void liberty_callback_draw(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0xff,0xff,0x00,0x00);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0xff);
	liberty_draw_font_string(font, "TESTING STRING: THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", (SDL_Point){5, 10});
	liberty_draw_font_string(font, "line 1\nline 2", (SDL_Point){20, 20});
	SDL_RenderPresent(renderer);
}

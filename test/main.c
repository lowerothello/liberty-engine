#include "../include/liberty.h"

LibertyFont *font;
LibertyConfig liberty_callback_init(void)
{
	font = liberty_new_font_from_file("test.bdf");
liberty_free_font(font);
exit(0);

	LibertyConfig ret;
	ret.title = "Liberty";
	ret.scale = 4;
	ret.width = font->w;
	ret.height = font->h;
	ret.framerate = 60.0f;
	ret.fullscreen = 0;
	ret.vsync = 0;
	return ret;
}

void liberty_callback_cleanup(void)
{
	liberty_free_font(font);
}

void liberty_callback_draw(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0xff,0xff,0xff,0xff);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0x00);
	SDL_RenderTexture(renderer, font->texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

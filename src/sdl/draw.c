void liberty_set_colour(LibertyColour colour)
{
	SDL_SetRenderDrawColor(Renderer, colour.r, colour.g, colour.b, colour.a);
}

void liberty_draw_clear(void)
{
	SDL_RenderClear(Renderer);
}

static void show_frame(void)
{
	liberty_set_draw_layer(NULL);
	SDL_RenderPresent(Renderer);
	liberty_set_colour((LibertyColour){0x00, 0x00, 0x00, 0xff});
	liberty_draw_clear();
}

void liberty_draw_pixels(SDL_FPoint *points, int count)
{
	SDL_RenderPoints(Renderer, points, count);
}

void liberty_draw_pixels_offset(SDL_FPoint *points, int count, SDL_Point pos)
{
	SDL_Rect rect;
	rect.x = pos.x;
	rect.y = pos.y;
	rect.w = Config.width - pos.x;
	rect.h = Config.height - pos.y;
	SDL_SetRenderViewport(Renderer, &rect);
	liberty_draw_pixels(points, count);
	SDL_SetRenderViewport(Renderer, NULL);
}

void liberty_draw_rect(bool fill, SDL_Rect rect)
{
	SDL_FRect frect = (SDL_FRect){rect.x, rect.y, rect.w, rect.h};
	if (fill) SDL_RenderFillRect(Renderer, &frect);
	else      SDL_RenderRect    (Renderer, &frect);
}

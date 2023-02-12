void liberty_set_colour(LibertyColour colour)
{
	SDL_SetRenderDrawColor(Renderer, colour.r, colour.g, colour.b, colour.a);
}
LibertyColour liberty_get_colour(void)
{
	LibertyColour colour;
	SDL_GetRenderDrawColor(Renderer, &colour.r, &colour.g, &colour.b, &colour.a);
	return colour;
}

void liberty_set_blend_mode(SDL_BlendMode blend)
{
	SDL_SetRenderDrawBlendMode(Renderer, blend);
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

void liberty_draw_pixels(LibertyVec2 *points, int count)
{
	SDL_RenderPoints(Renderer, (SDL_FPoint*)points, count); /* compatible type */
}
void liberty_draw_pixel(LibertyVec2 point)
{
	SDL_RenderPoint(Renderer, point.x, point.y);
}

void liberty_draw_pixels_offset(LibertyVec2 *points, int count, LibertyVec2 offset)
{
	SDL_Rect rect;
	rect.x = offset.x;
	rect.y = offset.y;
	rect.w = Config.width - offset.x;
	rect.h = Config.height - offset.y;
	SDL_SetRenderViewport(Renderer, &rect);
	liberty_draw_pixels(points, count);
	SDL_SetRenderViewport(Renderer, NULL);
}

void liberty_draw_line(LibertyVec2 start, LibertyVec2 end)
{
	SDL_RenderLine(Renderer, start.x, start.y, end.x, end.y);
}

void liberty_draw_rect(bool fill, LibertyVec4 rect)
{
	if (fill) SDL_RenderFillRect(Renderer, (SDL_FRect*)&rect); /* compatible type */
	else      SDL_RenderRect    (Renderer, (SDL_FRect*)&rect); /* compatible type */
}

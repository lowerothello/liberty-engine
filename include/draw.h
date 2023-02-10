void liberty_set_colour(LibertyColour colour);
void liberty_draw_clear(void);

void liberty_draw_pixels(SDL_FPoint *points, int count);
void liberty_draw_pixels_offset(SDL_FPoint *points, int count, SDL_Point pos);

void liberty_draw_rect(bool fill, SDL_Rect rect);

typedef struct LibertyColour
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} LibertyColour;

void liberty_set_colour(LibertyColour colour);
LibertyColour liberty_get_colour(void);
void liberty_set_blend_mode(SDL_BlendMode blend);
void liberty_draw_clear(void);

void liberty_draw_pixels(LibertyVec2 *points, int count);
void liberty_draw_pixels_offset(LibertyVec2 *points, int count, LibertyVec2 offset);
void liberty_draw_pixel(LibertyVec2 point);

void liberty_draw_line(LibertyVec2 start, LibertyVec2 end);

void liberty_draw_rect(bool fill, LibertyVec4 rect);

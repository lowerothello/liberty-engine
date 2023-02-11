void liberty_set_colour(LibertyColour colour);
void liberty_draw_clear(void);

void liberty_draw_pixels(LibertyVec2 *points, int count);
void liberty_draw_pixels_offset(LibertyVec2 *points, int count, LibertyVec2 offset);
void liberty_draw_pixel(LibertyVec2 point);

void liberty_draw_line(LibertyVec2 start, LibertyVec2 end);

void liberty_draw_rect(bool fill, LibertyVec4 rect);

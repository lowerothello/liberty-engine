/* all drawing operations use the active colour and draw to the active layer */

typedef struct LibertyColour
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} LibertyColour;

/* set/get the active colour */
void liberty_set_colour(LibertyColour colour);
LibertyColour liberty_get_colour(void);

/* fill */
void liberty_draw_clear(void);

/* draw some pixels */
/* .points is an array of pixels to draw */
/* .count is how many pixels to expect in .points */
/* .offset is the offset on the screen to draw the pixels at */
void liberty_draw_pixels(LibertyVec2 *points, int count, LibertyVec2 offset);

/* draw a pixel */
void liberty_draw_pixel(LibertyVec2 point);

/* draw a line */
void liberty_draw_line(LibertyVec2 start, LibertyVec2 end);

/* draw a rectangle */
/* .fill is whether or not to fill in the rectangle */
/*     FALSE for outline */
/*     TRUE for solid */
void liberty_draw_rect(bool fill, LibertyRect rect);

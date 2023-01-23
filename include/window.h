typedef struct {
	/* unsafe to modify directly, use liberty_resize_window() */
	uint8_t     scale;
	uint16_t    width, height;
	uint16_t    framerate;
	/* internal */
	float       frametime;
	float       frametimeelapsed;
	SDL_Window *window;
} LibertyWindow;


/* create a liberty window.
 *   .title is the window title
 *   .scale is the size of each LibertyPixel
 *   .width is the window width
 *   .height is the window height
 *   .framerate is the target framerate or 0 for unlimited
 */
LibertyWindow *liberty_create_window(const char *title,
		uint8_t scale,
		uint16_t width,
		uint16_t height,
		uint16_t framerate);

/* resize a liberty window.
 *   .lwin is the window to modify.
 *   .title is the new title, NULL for unchanged.
 *   .scale is the new size of each LibertyPixel, l->scale for unchanged.
 *   .width is the new window width, l->width for unchanged.
 *   .height is the new window height, l->height for unchanged.
 *   .framerate is the new target framerate or 0 for unlimited, l->framerate for unchanged.
 */
void liberty_resize_window(LibertyWindow *lwin,
		const char *title,
		uint8_t scale,
		uint16_t width,
		uint16_t height,
		uint16_t framerate);

/* destroy a liberty window.
 *   .lwin is the window to destroy, it is invalidated by this function.
 */
void liberty_destroy_window(LibertyWindow *lwin);

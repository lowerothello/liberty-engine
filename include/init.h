/*
 * initialize a new liberty instance.
 * creates the window and returns global state.
 *   .title is the window title
 *   .scale is the size of each LibertyPixel
 *   .width is the window width
 *   .height is the window height
 *   .framerate is the target framerate or 0 for unlimited
 */
Liberty *liberty_init(const char *title,
		uint8_t scale,
		uint16_t width,
		uint16_t height,
		uint16_t framerate);

/*
 * resize a liberty instance.
 *   .title is the new title, NULL for unchanged
 *   .scale is the new size of each LibertyPixel, l->scale for unchanged
 *   .width is the new window width, l->width for unchanged
 *   .height is the new window height, l->height for unchanged
 *   .framerate is the new target framerate or 0 for unlimited, l->framerate for unchanged
 */
void liberty_resize(Liberty *l,
		const char *title,
		uint8_t scale,
		uint16_t width,
		uint16_t height,
		uint16_t framerate);

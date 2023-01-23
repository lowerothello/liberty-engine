static void update_frametime(LibertyWindow *l)
{
	if (!l->framerate)
		l->frametime = 0.0f; /* avoid divide by 0 */
	else
		l->frametime = (1.0f / (float)l->framerate) * 1000.0f; /* based around milliseconds */
}

LibertyWindow *liberty_create_window(const char *title,
		uint8_t scale,
		uint16_t width,
		uint16_t height,
		uint16_t framerate)
{
	LibertyWindow *lwin = calloc(1, sizeof(LibertyWindow));
	lwin->scale = scale;
	lwin->width = width;
	lwin->height = height;
	lwin->framerate = framerate;
	update_frametime(lwin);
	lwin->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*scale, height*scale, 0);
LOG("created LibertyWindow %p\n", lwin);
	return lwin;
}

void liberty_resize_window(LibertyWindow *lwin,
		const char *title,
		uint8_t scale,
		uint16_t width,
		uint16_t height,
		uint16_t framerate)
{
	if (title) SDL_SetWindowTitle(lwin->window, title);
	lwin->scale = scale;
	lwin->width = width;
	lwin->height = height;
	lwin->framerate = framerate;
	update_frametime(lwin);
	SDL_SetWindowSize(lwin->window, width*scale, height*scale);
LOG("resized LibertyWindow %p\n", lwin);
}

void liberty_destroy_window(LibertyWindow *lwin)
{
LOG("destroying LibertyWindow %p\n", lwin);
	SDL_DestroyWindow(lwin->window);
	free(lwin);
}

static void update_frametime(LibertyWindow *l)
{
	if (!l->framerate)
		l->frametime = 0.0f; /* avoid divide by 0 */
	else
		l->frametime = (1.0f / (float)l->framerate) * 1000.0f; /* based around milliseconds */
}

Liberty *liberty_create_window(const char *title,
		uint8_t scale,
		uint16_t width,
		uint16_t height,
		uint16_t framerate)
{
	Liberty *lwin = calloc(1, sizeof(Liberty));
	lwin->scale = scale;
	lwin->width = width;
	lwin->height = height;
	lwin->framerate = framerate;
	liberty_update_frametime(l);
	l->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*scale, height*scale, 0);
	return l;
}

void liberty_resize_window(LibertyWindow *lwin,
		const char *title,
		uint8_t scale,
		uint16_t width,
		uint16_t height,
		uint16_t framerate)
{
	if (title) SDL_SetWindowTitle(l->window, title);
	l->scale = scale;
	l->width = width;
	l->height = height;
	l->framerate = framerate;
	liberty_update_frametime(l);
	SDL_SetWindowSize(l->window, width*scale, height*scale);
}

void liberty_destroy_window(LibertyWindow *lwin)
{
	SDL_DestroyWindow(lwin->window);
	free(lwin);
}

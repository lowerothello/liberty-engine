#define SDL_WINDOW_FLAGS 0
void create_window(LibertyConfig config)
{
	SDL_CreateWindowAndRenderer(
			config.width*config.scale,
			config.height*config.scale,
			SDL_WINDOW_FLAGS,
			&Window,
			&Renderer);
	SDL_SetWindowTitle(Window, config.title);
LOG("created SDL_Window *'%s' (%p)\n", config.title, Window);
	/* hints */
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
}

void resize_window(LibertyConfig config)
{
LOG("resizing SDL_Window *'%s' (%p)\n", config.title, Window);
	SDL_SetWindowTitle(Window, config.title);
	SDL_SetWindowSize(Window,
			config.width*config.scale,
			config.height*config.scale);
}

void destroy_window(SDL_Window *window)
{
LOG("destroying SDL_Window *(%p)\n", window);
	SDL_DestroyWindow(window);
}

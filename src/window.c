SDL_Window *create_window(LibertyConfig config)
{
	SDL_Window *window = SDL_CreateWindow(config.title,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			config.width*config.scale,
			config.height*config.scale, 0);
LOG("created SDL_Window *'%s' (%p)\n", config.title, window);
	return window;
}

void resize_window(SDL_Window *window, LibertyConfig config)
{
LOG("resizing SDL_Window *'%s' (%p)\n", config.title, window);
	SDL_SetWindowTitle(window, config.title);
	SDL_SetWindowSize(window,
			config.width*config.scale,
			config.height*config.scale);
}

void destroy_window(SDL_Window *window)
{
LOG("destroying SDL_Window *(%p)\n", window);
	SDL_DestroyWindow(window);
}

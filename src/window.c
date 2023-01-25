#define SDL_WINDOW_FLAGS 0
/* allocates Window and Renderer */
void create_window(void)
{
	/* TODO: proper fullscreen modesetting */
	SDL_CreateWindowAndRenderer(
			Config.width*Config.scale,
			Config.height*Config.scale,
			SDL_WINDOW_FLAGS | (Config.fullscreen && SDL_WINDOW_FULLSCREEN),
			&Window,
			&Renderer);
	SDL_SetRenderLogicalSize(Renderer, Config.width, Config.height);
	SDL_SetRenderIntegerScale(Renderer, 1);
	SDL_SetRenderVSync(Renderer, Config.vsync);
	SDL_SetWindowTitle(Window, Config.title);
LOG("created SDL_Window* %p\n", Window);

	/* hints */
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
}

void resize_window(void)
{
LOG("resizing SDL_Window* %p\n", Window);
	SDL_SetWindowTitle(Window, Config.title);
	SDL_SetWindowFullscreen(Window, Config.fullscreen);
	SDL_SetWindowSize(Window,
			Config.width*Config.scale,
			Config.height*Config.scale);
	SDL_SetRenderVSync(Renderer, Config.vsync);
}

void destroy_window(void)
{
LOG("destroying SDL_Window* %p\n", Window);
	SDL_DestroyWindow(Window); Window = NULL;
}

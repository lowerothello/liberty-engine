/* allocates Window */
int create_window(void)
{
	/* TODO: proper fullscreen modesetting */
	SDL_CreateWindowAndRenderer(
			Config.width*Config.scale,
			Config.height*Config.scale,
			SDL_WINDOW_RESIZABLE,
			&Window,
			&Renderer);
	if (!Window)
	{
		ERR("Window failed to initialize");
		ERR(SDL_GetError());
		return 1;
	}
	if (!Renderer)
	{
		ERR("Renderer failed to initialize");
		ERR(SDL_GetError());
		return 1;
	}
	SDL_SetWindowFullscreen(Window, Config.fullscreen);
	SDL_SetRenderVSync(Renderer, Config.vsync);
	SDL_SetRenderLogicalSize(Renderer, Config.width, Config.height);
	SDL_SetRenderIntegerScale(Renderer, 1);
	LOG("created SDL_Window* %p\n", Window);
	LOG("created SDL_Renderer* %p\n", Renderer);
	return 0;
}

void resize_window(void)
{
	LOG("resizing SDL_Window* %p\n", Window);
	SDL_SetWindowTitle(Window, Config.title);
	SDL_SetWindowFullscreen(Window, Config.fullscreen);
	SDL_SetWindowSize(Window, Config.width, Config.height);
	SDL_SetRenderVSync(Renderer, Config.vsync);
}

void destroy_window(void)
{
	LOG("destroying SDL_Renderer* %p\n", Renderer);
	SDL_DestroyRenderer(Renderer); Renderer = NULL;
	LOG("destroying SDL_Window* %p\n", Window);
	SDL_DestroyWindow(Window); Window = NULL;
}



/* allocates Window and Renderer */
/* returns non-zero on error */
int create_window(void)
{
	/* TODO: proper fullscreen modesetting */
	SDL_CreateWindowAndRenderer(
			Config.width,
			Config.height,
			SDL_WINDOW_RESIZABLE,
			&Window,
			&Renderer);
	SDL_SetWindowFullscreen(Window, Config.fullscreen);
	SDL_SetRenderLogicalSize(Renderer, Config.width, Config.height);
	SDL_SetRenderIntegerScale(Renderer, 1);
	SDL_SetRenderVSync(Renderer, Config.vsync);
	SDL_SetWindowTitle(Window, Config.title);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
LOG("created SDL_Window* %p\n", Window);

	SDL_RendererInfo info;
	SDL_GetRendererInfo(Renderer, &info);
LOG("created SDL_Renderer* %p %s 0x%08x\n", Renderer, info.name, info.flags);
	if (!(info.flags&SDL_RENDERER_TARGETTEXTURE))
	{
		printf("ERROR: renderer %s doesn't support drawing to texture\n", info.name);
		return -1;
	}
	return 0;
}

void resize_window(void)
{
LOG("resizing SDL_Window* %p\n", Window);
	SDL_SetWindowTitle(Window, Config.title);
	SDL_SetWindowFullscreen(Window, Config.fullscreen);
	SDL_SetWindowSize(Window,
			Config.width,
			Config.height);
	SDL_SetRenderLogicalSize(Renderer, Config.width, Config.height);
	SDL_SetRenderVSync(Renderer, Config.vsync);
}

void destroy_window(void)
{
LOG("destroying SDL_Window* %p\n", Window);
	SDL_DestroyWindow(Window); Window = NULL;
}

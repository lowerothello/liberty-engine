SDL_Texture *liberty_new_layer(void)
{
	SDL_RendererInfo info;
	SDL_GetRendererInfo(Renderer, &info);
	return SDL_CreateTexture(Renderer, info.texture_formats[0],
			SDL_TEXTUREACCESS_TARGET, Config.width, Config.height);
}

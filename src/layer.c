SDL_Texture *liberty_new_screen_texture(SDL_Renderer *r)
{
	SDL_RendererInfo info;
	SDL_GetRendererInfo(r, &info);
	return SDL_CreateTexture(r, info.texture_formats[0],
			SDL_TEXTUREACCESS_TARGET, Config.width, Config.height);
}

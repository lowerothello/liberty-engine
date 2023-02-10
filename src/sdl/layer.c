LibertyLayer *liberty_new_layer(void)
{
	SDL_RendererInfo info;
	SDL_GetRendererInfo(Renderer, &info);
	SDL_Texture *texture = SDL_CreateTexture(Renderer, info.texture_formats[0],
			SDL_TEXTUREACCESS_TARGET, Config.width, Config.height);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
	return texture;
}

void liberty_free_layer(LibertyLayer *layer)
{
	SDL_DestroyTexture(layer);
}

/* set to NULL to draw directly to the screen */
void liberty_set_draw_layer(LibertyLayer *layer)
{
	SDL_SetRenderTarget(Renderer, layer);
}

void liberty_draw_layer(LibertyLayer *layer,
		SDL_Point offset, LibertyColour tint)
{
	SDL_SetTextureColorMod(layer, tint.r, tint.g, tint.b);
	SDL_SetTextureAlphaMod(layer, tint.a);
	SDL_FRect destrect = {offset.x, offset.y, Config.width, Config.height};
	SDL_RenderTexture(Renderer, layer, NULL, &destrect);
}

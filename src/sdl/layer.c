LibertyLayer *liberty_new_layer(LibertyVec2 size)
{
	SDL_RendererInfo info;
	SDL_GetRendererInfo(Renderer, &info);
	SDL_Texture *texture = SDL_CreateTexture(Renderer, info.texture_formats[0],
			SDL_TEXTUREACCESS_TARGET, size.x, size.y);
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
		SDL_BlendMode blend,
		LibertyVec2 offset)
{
	SDL_SetTextureBlendMode(layer, blend);
	LibertyRGB colour = liberty_get_colour();
	SDL_SetTextureColorMod(layer, colour.r, colour.g, colour.b);
	SDL_SetTextureAlphaMod(layer, colour.a);
	SDL_Rect srcrect = {offset.x, offset.y, Config.width, Config.height};
	SDL_RenderTexture(Renderer, layer, &srcrect, NULL);
}

void liberty_draw_layer_row(LibertyLayer *layer, uint16_t row,
		SDL_BlendMode blend,
		LibertyVec2 offset, float scale)
{
	SDL_SetTextureBlendMode(layer, blend);
	LibertyRGB colour = liberty_get_colour();
	SDL_SetTextureColorMod(layer, colour.r, colour.g, colour.b);
	SDL_SetTextureAlphaMod(layer, colour.a);
	SDL_Rect srcrect = {offset.x, row + offset.y, Config.width, 1};
	float width = Config.width * scale;
	SDL_FRect destrect = {(Config.width - width)*0.5f, row, width, 1};
	SDL_RenderTexture(Renderer, layer, &srcrect, &destrect);
}

void liberty_draw_layer_column(LibertyLayer *layer, uint16_t column,
		SDL_BlendMode blend,
		LibertyVec2 offset, float scale)
{
	SDL_SetTextureBlendMode(layer, blend);
	LibertyRGB colour = liberty_get_colour();
	SDL_SetTextureColorMod(layer, colour.r, colour.g, colour.b);
	SDL_SetTextureAlphaMod(layer, colour.a);
	SDL_Rect srcrect = {column + offset.x, offset.y, 1, Config.height};
	float height = Config.height * scale;
	SDL_FRect destrect = {column, (Config.height - height)*0.5f, 1, height};
	SDL_RenderTexture(Renderer, layer, &srcrect, &destrect);
}

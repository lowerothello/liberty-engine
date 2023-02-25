LibertyLayer *liberty_new_layer(LibertyVec2 size)
{
	LibertyLayer *ret = malloc(sizeof(LibertyLayer));
	ret->width = size.x;
	ret->height = size.y;

	SDL_RendererInfo info;
	SDL_GetRendererInfo(Renderer, &info);
	ret->data = SDL_CreateTexture(Renderer, info.texture_formats[0],
			SDL_TEXTUREACCESS_TARGET, ret->width, ret->height);
	return ret;
}

void liberty_free_layer(LibertyLayer *layer)
{
	SDL_DestroyTexture(layer->data);
	free(layer);
}

LibertyLayer *RenderTarget = NULL;
/* set to NULL to draw directly to the screen */
void liberty_set_draw_layer(LibertyLayer *layer)
{
	RenderTarget = layer;

	if (layer)
		SDL_SetRenderTarget(Renderer, layer->data);
	else
		SDL_SetRenderTarget(Renderer, NULL);
}

void liberty_draw_layer(LibertyLayer *layer,
		SDL_BlendMode blend,
		LibertyVec2 offset)
{
	SDL_SetTextureBlendMode(layer->data, blend);
	LibertyRGB colour = liberty_get_colour();
	SDL_SetTextureColorMod(layer->data, colour.r, colour.g, colour.b);
	SDL_SetTextureAlphaMod(layer->data, colour.a);

	SDL_Rect srcrect;
	if (RenderTarget) srcrect = (SDL_Rect){offset.x, offset.y, RenderTarget->width, RenderTarget->height};
	else              srcrect = (SDL_Rect){offset.x, offset.y, Config.width, Config.height};

	SDL_RenderTexture(Renderer, layer->data, &srcrect, NULL);
}

void liberty_draw_layer_row(LibertyLayer *layer, uint16_t row,
		SDL_BlendMode blend,
		LibertyVec2 offset, float scale)
{
	SDL_SetTextureBlendMode(layer->data, blend);
	LibertyRGB colour = liberty_get_colour();
	SDL_SetTextureColorMod(layer->data, colour.r, colour.g, colour.b);
	SDL_SetTextureAlphaMod(layer->data, colour.a);

	SDL_Rect srcrect;
	SDL_FRect destrect;
	float width;
	if (RenderTarget)
	{
		srcrect = (SDL_Rect){offset.x, row + offset.y, RenderTarget->width, 1};
		width = RenderTarget->width * scale;
		destrect = (SDL_FRect){(RenderTarget->width - width)*0.5f, row, width, 1};
	} else
	{
		srcrect = (SDL_Rect){offset.x, row + offset.y, Config.width, 1};
		width = Config.width * scale;
		destrect = (SDL_FRect){(Config.width - width)*0.5f, row, width, 1};
	}

	SDL_RenderTexture(Renderer, layer->data, &srcrect, &destrect);
}

void liberty_draw_layer_column(LibertyLayer *layer, uint16_t column,
		SDL_BlendMode blend,
		LibertyVec2 offset, float scale)
{
	SDL_SetTextureBlendMode(layer->data, blend);
	LibertyRGB colour = liberty_get_colour();
	SDL_SetTextureColorMod(layer->data, colour.r, colour.g, colour.b);
	SDL_SetTextureAlphaMod(layer->data, colour.a);

	SDL_Rect srcrect;
	SDL_FRect destrect;
	float height;
	if (RenderTarget)
	{
		srcrect = (SDL_Rect){column + offset.x, offset.y, 1, RenderTarget->height};
		height = RenderTarget->height * scale;
		destrect = (SDL_FRect){column, (RenderTarget->height - height)*0.5f, 1, height};
	} else
	{
		srcrect = (SDL_Rect){column + offset.x, offset.y, 1, Config.height};
		height = Config.height * scale;
		destrect = (SDL_FRect){column, (Config.height - height)*0.5f, 1, height};
	}

	SDL_RenderTexture(Renderer, layer->data, &srcrect, &destrect);
}

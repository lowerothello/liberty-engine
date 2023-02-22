LibertyTexture *liberty_new_texture_from_file(const char *path)
{
	LibertyTexture *ret = calloc(1, sizeof(LibertyTexture));

	unsigned int error;
	unsigned char *image = 0;

	error = lodepng_decode32_file(&image, &ret->w, &ret->h, path);
	if (error)
	{
		LOG("LODEPNG: error %u: %s\n", error, lodepng_error_text(error));
		return NULL;
	}

	SDL_Surface *surface = SDL_CreateSurfaceFrom(image, ret->w, ret->h, ret->w<<2 /* *4 */, SDL_PIXELFORMAT_ARGB8888);
	if (!surface) { LOG("SDL Error: %s\n", SDL_GetError()); }

	ret->image = SDL_CreateTextureFromSurface(Renderer, surface);
	if (!ret->image) { LOG("SDL Error: %s\n", SDL_GetError()); }

	SDL_SetTextureBlendMode(ret->image, SDL_BLENDMODE_BLEND);

	SDL_DestroySurface(surface);
	free(image);

	return ret;
}

/* returns the new sprite index */
/* reallocs texture internally */
uint32_t liberty_add_texture_sprite(LibertyTexture **texture, LibertyIntRect sprite)
{
	(*texture)->spritecount++;
	*texture = realloc(*texture, sizeof(LibertyTexture) + sizeof(LibertyRect) * (*texture)->spritecount);

	(*texture)->sprite[(*texture)->spritecount - 1] = sprite;
	return (*texture)->spritecount - 1;
}

void liberty_free_texture(LibertyTexture *texture)
{
	SDL_DestroyTexture(texture->image);
	free(texture);
}

void liberty_draw_texture(LibertyTexture *texture, LibertyVec2 pos, float rotate, bool hflip, bool vflip)
{
	LibertyRGB colour = liberty_get_colour();
	SDL_SetTextureColorMod(texture->image, colour.r, colour.g, colour.b);
	SDL_SetTextureAlphaMod(texture->image, colour.a);
	SDL_FRect dest = (SDL_FRect){pos.x, pos.y, texture->w, texture->h};
	SDL_RenderTextureRotated(Renderer, texture->image, NULL, &dest, rotate, NULL, (int)hflip + ((int)vflip<<1));
}

void liberty_draw_texture_sprite(LibertyTexture *texture, uint32_t sprite, LibertyVec2 pos, float rotate, bool hflip, bool vflip)
{
	LibertyRGB colour = liberty_get_colour();
	SDL_SetTextureColorMod(texture->image, colour.r, colour.g, colour.b);
	SDL_SetTextureAlphaMod(texture->image, colour.a);
	SDL_FRect dest = (SDL_FRect){pos.x, pos.y, texture->w, texture->h};
	SDL_RenderTextureRotated(Renderer, texture->image, (SDL_Rect*)&texture->sprite[sprite], &dest, rotate, NULL, (int)hflip + ((int)vflip<<1));
}

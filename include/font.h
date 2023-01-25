typedef struct LibertyGlyph
{
	SDL_Rect bbx;
	int      offset; /* horizontal offset through the texture */
} LibertyGlyph;
typedef struct LibertyFont
{
	char        *name;
	int          w, h;
	LibertyGlyph glyph[128];
	SDL_Texture *texture;
} LibertyFont;

/*
 * .path should point to a bdf file.
 * returns NULL on failure.
 */
LibertyFont *liberty_new_font_from_file(const char *path);
void liberty_free_font(LibertyFont *font);

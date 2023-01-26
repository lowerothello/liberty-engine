/* this api will fail if used before the window is initialized! */

typedef struct LibertyGlyph
{
	SDL_Rect    bbx;
	int         count;
	SDL_FPoint *points;
} LibertyGlyph;
typedef struct LibertyFont
{
	char        *name;
	int          h;
	LibertyGlyph glyph[128];
} LibertyFont;

/*
 * .path should point to a bdf file.
 * returns NULL on failure.
 */
LibertyFont *liberty_new_font_from_file(const char *path);
void liberty_free_font(LibertyFont *font);

SDL_Point liberty_font_char(LibertyFont *font, int glyph, SDL_Point pos);
SDL_Point liberty_font_string(LibertyFont *font, char *string, SDL_Point pos);

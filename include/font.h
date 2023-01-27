/*
 * liberty font library
 *   loads and renders bdf fonts
 */

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

/* .path should point to a bdf file, returns NULL on failure. */
LibertyFont *liberty_new_font_from_file(const char *path);

/* free's and invalidates .font */
void liberty_free_font(LibertyFont *font);

/* render a string using font .font */
SDL_Point liberty_draw_font_string(LibertyFont *font, char *string, SDL_Point pos);

/* get the bounding box needed to render a string in */
SDL_Rect liberty_get_font_string_bbx(LibertyFont *font, char *string, SDL_Point pos);

/*
 * liberty font library
 *   loads and renders bdf fonts
 */

typedef struct LibertyGlyph
{
	LibertyIntVec4  bbx;
	int             count;
	LibertyVec2    *points;
} LibertyGlyph;
typedef struct LibertyFont
{
	char        *name;
	int          h;
	float        frame; /* used for animations */
	LibertyGlyph glyph[128];
} LibertyFont;

/* .path should point to a bdf file, returns NULL on failure. */
LibertyFont *liberty_new_font_from_file(const char *path);

/* free's and invalidates .font */
void liberty_free_font(LibertyFont *font);

/* render a string using font .font */
LibertyVec2 liberty_draw_font_string        (LibertyFont *font, LibertyVec2 pos, char *string);
LibertyVec2 liberty_draw_font_string_outline(LibertyFont *font, LibertyVec2 pos, char *string);

void liberty_draw_font_animation(LibertyFont *font, LibertyVec2 pos, float rate);

/* get the bounding box needed to render a string in */
LibertyVec4 liberty_get_font_string_bbx(LibertyFont *font, LibertyVec2 pos, char *string);

LibertyVec2 liberty_get_font_string_centre(LibertyFont *font, LibertyVec4 rect, char *string);

#define LIBERTY_DRAW_FONT_STRING_CENTRE(font, rect, string) \
	liberty_draw_font_string(font, liberty_get_font_string_centre(font, rect, string), string);

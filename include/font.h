/* loads and renders bdf fonts */

/* a single font glyph */
typedef struct LibertyGlyph
{
	LibertyIntRect bbx;    /* glyph bounding box */
	int            count;  /* number of pixels that make up the glyph */
	LibertyVec2   *points; /* pixels that make up the glyph */
} LibertyGlyph;

/* an entire font worth of glyphs */
typedef struct LibertyFont
{
	char        *name;       /* font name */
	int          h;          /* font height */
	float        frame;      /* used for animations */
	LibertyGlyph glyph[128]; /* glyphs, up to ascii delete */
} LibertyFont;

/* reads a font in from a file */
/* .path should point to a valid bdf file, returns NULL on failure. */
LibertyFont *liberty_new_font_from_file(const char *path);

/* free's and invalidates .font */
void liberty_free_font(LibertyFont *font);


/* get the bounding box that would be drawn to with an identical call to liberty_draw_font_string */
/* doesn't actually draw anything to the screen */
LibertyRect liberty_get_font_string_bbx(LibertyFont *font, LibertyVec2 pos, char *string);

/* returns the pos that would be needed to draw .string in the centre of bounding box .rect */
LibertyVec2 liberty_get_font_string_centre(LibertyFont *font, LibertyRect rect, char *string);


/* draw a string */
/* .pos is the top left corner to start drawing from */
/* returns the bottom right corner of the string */
LibertyVec2 liberty_draw_font_string(LibertyFont *font, LibertyVec2 pos, char *string);
#define LIBERTY_DRAW_FONT_STRING_CENTRE(font, rect, string) liberty_draw_font_string(font, liberty_get_font_string_centre(font, rect, string), string);

/* draw a string in a box */
LibertyVec2 liberty_draw_font_string_outline(LibertyFont *font, LibertyVec2 pos, char *string);
#define LIBERTY_DRAW_FONT_STRING_OUTLINE_CENTRE(font, rect, string) liberty_draw_font_string_outline(font, liberty_get_font_string_centre(font, rect, string), string);

/* draw a string with reverse colours */
LibertyVec2 liberty_draw_font_string_reverse(LibertyFont *font, LibertyVec2 pos, char *string);
#define LIBERTY_DRAW_FONT_STRING_REVERSE_CENTRE(font, rect, string) liberty_draw_font_string_reverse(font, liberty_get_font_string_centre(font, rect, string), string);

/* draw .count glyphs of a string */
LibertyVec2 liberty_draw_font_string_count(LibertyFont *font, LibertyVec2 pos, char *string, size_t count);
#define LIBERTY_DRAW_FONT_STRING_COUNT_CENTRE(font, rect, string, count) liberty_draw_font_string_count(font, liberty_get_font_string_centre(font, rect, string), string, count);

/* show glyphs in a font one after the other */
/* rate should be relative to deltatime */
void liberty_draw_font_animation(LibertyFont *font, LibertyVec2 pos, float rate);

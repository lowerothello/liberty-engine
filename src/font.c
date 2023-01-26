#define FILE_BUFFER_CHUNK_SIZE 256
/* returns the last char read, which is always out of .ifs or EOF */
/* .buffer is allowed to be NULL */
static int read_file_until_ifs(char **buffer, FILE *fp, const char *ifs)
{
	size_t ptr = 0;
	size_t ifsptr;
	int byte = 0;
	if (buffer) *buffer = malloc(FILE_BUFFER_CHUNK_SIZE);
	while (1)
	{
		byte = fgetc(fp);
		for (ifsptr = 0; ifsptr < strlen(ifs); ifsptr++)
			if (byte == ifs[ifsptr])
				goto byte_eof;

		if (byte == EOF)
		{
byte_eof:
			if (buffer) (*buffer)[ptr] = '\0';
			break;
		}

		if (buffer)
		{
			(*buffer)[ptr] = byte;
			ptr++;
			if (!ptr%FILE_BUFFER_CHUNK_SIZE)
				*buffer = realloc(*buffer, ptr+FILE_BUFFER_CHUNK_SIZE); /* allocate another chunk if needed */
		}
	}
	if (buffer) *buffer = realloc(*buffer, strlen(*buffer)+1);
	return byte;
}

static int read_file_until_string(FILE *fp, const char *end)
{
	size_t ptr = 0;
	int byte = 0;
	while (1)
	{
		byte = fgetc(fp);
		if (byte == EOF)
			return EOF;

		if (byte == end[ptr])
			ptr++;
		else
			ptr = 0;

		if (ptr == strlen(end))
			return 0;
	}
}

/* https://stackoverflow.com/a/20462958 */
// static unsigned int hash_string(char *string)
// {
// 	unsigned int hash = 0;
// 	for (size_t i = 0; string[i] != '\0'; i++)
// 		hash = 31 * hash + string[i];
//
// 	return hash;
// }


static void read_bdf_string(char **dest, FILE *fp)
{
	read_file_until_ifs(NULL, fp, "\"");
	read_file_until_ifs(dest, fp, "\"");
}

static int read_bdf_number(int *dest, FILE *fp, const char *delimiter)
{
	char *buffer;
	int ret = read_file_until_ifs(&buffer, fp, delimiter);
	*dest = strtol(buffer, NULL, 10);
	free(buffer);
	return ret;
}

// static void read_hex_bitmap(char *string, LibertyGlyph *glyph, int y)
// {
// 	long integer = strtol(string, NULL, 16);
//
// 	size_t bits = strlen(string)*4; /* every bit */
// 	for (size_t i = 0; i < bits; i++)
// 		if (integer&(1<<(bits - (i+1)))) { LOG("█"); }
// 		else                             { LOG(" "); }
// }

/* font->h is expected to be correct */
static void parse_bdf_char(LibertyFont *font, FILE *fp)
{
	char *keyword;
	char *hexline;
	int index = 0;
	int ret; /* TODO: should probably return this */
	long integer;
	int i, j;

	/* position the file pointer after the startchar line */
	read_file_until_string(fp, "STARTCHAR"); read_file_until_ifs(NULL, fp, "\n");
LOG("BDF: STARTCHAR\n");

	while (1)
	{
		ret = read_file_until_ifs(&keyword, fp, " \n"); /* get the keyword by reading until the first whitespace */

		if (!strcmp(keyword, "ENCODING"))
		{
			read_bdf_number(&index, fp, "\n");
LOG("BDF: ENCODING '%c'\n", index);
		} else if (!strcmp(keyword, "BBX"))
		{
			ret = read_bdf_number(&font->glyph[index].bbx.w, fp, " \n" );
			ret = read_bdf_number(&font->glyph[index].bbx.h, fp, " \n" );
			ret = read_bdf_number(&font->glyph[index].bbx.x, fp, " \n" );
			ret = read_bdf_number(&font->glyph[index].bbx.y, fp, " \n" );
			if (ret != '\n') read_file_until_ifs(NULL, fp, "\n");
			font->glyph[index].bbx.y = font->h - (font->glyph[index].bbx.y + font->glyph[index].bbx.h);
LOG("BDF: BBX: w=%d h=%d x=%d y=%d\n",
	font->glyph[index].bbx.w, font->glyph[index].bbx.h,
	font->glyph[index].bbx.x, font->glyph[index].bbx.y);
		} else if (!strcmp(keyword, "BITMAP"))
		{
LOG("BDF: BITMAP\n");
			if (ret != '\n') read_file_until_ifs(NULL, fp, "\n");

			/* oversized buffer */
			font->glyph[index].points = malloc(sizeof(SDL_FPoint) * font->glyph[index].bbx.w * font->glyph[index].bbx.h);
			font->glyph[index].count = 0;

			for (i = 0; i < font->glyph[index].bbx.h; i++)
			{
				read_file_until_ifs(&hexline, fp, "\n");
				// read_hex_bitmap(hexline, &font->glyph[index], i);
				integer = strtol(hexline, NULL, 16);

				for (j = 0; j < strlen(hexline)<<2; j++)
					if (integer&(1<<((strlen(hexline)<<2) - (j+1))))
					{ LOG("█");
						font->glyph[index].points[font->glyph[index].count].x = j;
						font->glyph[index].points[font->glyph[index].count].y = i;
						font->glyph[index].count++;
					}
					else { LOG(" "); }
LOG("\n");
				free(hexline);
			}
			font->glyph[index].points = realloc(font->glyph[index].points, sizeof(SDL_FPoint) * font->glyph[index].count);
LOG("\n\n");
		} else if (!strcmp(keyword, "ENDCHAR"))
		{
LOG("BDF: ENDCHAR\n");
			if (ret != '\n') read_file_until_ifs(NULL, fp, "\n");
			free(keyword);
			break;
		} else
			if (ret != '\n') read_file_until_ifs(NULL, fp, "\n");
		free(keyword);
	}
}

/* returns EOF when the end of the file is reached and 0 otherwise */
static void parse_bdf_line(LibertyFont *font, FILE *fp)
{
	char *keyword;
	int ret;
	while (1)
	{
		ret = read_file_until_ifs(&keyword, fp, " \n"); /* get the keyword by reading until the first space */
		if (!strcmp(keyword, "FAMILY_NAME"))
		{
LOG("BDF: FAMILY_NAME\n");
			read_bdf_string(&font->name, fp);
			read_file_until_ifs(NULL, fp, "\n");
		} else if (!strcmp(keyword, "PIXEL_SIZE"))
		{
LOG("BDF: PIXEL_SIZE\n");
			read_bdf_number(&font->h, fp, "\n");
		} else if (!strcmp(keyword, "CHARS"))
		{
LOG("BDF: CHARS\n");
			int i, charcount;
			read_bdf_number(&charcount, fp, "\n");
LOG("BDF: %d glyphs in font\n", charcount);

			for (i = 0; i < charcount; i++)
				parse_bdf_char(font, fp);
		} else if (!strcmp(keyword, "ENDFONT"))
		{
LOG("BDF: ENDFONT\n");
			free(keyword);
			return;
		} else
		{
			if (ret != '\n' && read_file_until_ifs(NULL, fp, "\n") == EOF)
			{
				free(keyword);
				return;
			}
		}
		free(keyword);
	}
	return;
}

LibertyFont *liberty_new_font_from_file(const char *path)
{
	if (!Renderer)
	{
		LOG("BDF: failed to open font\n");
		return NULL;
	}
LOG("BDF: opening file %s\n", path);
	FILE *fp = fopen(path, "r");
	if (!fp)
	{
LOG("file %s doesn't exist\n", path);
LOG("BDF: failed to open font\n");
		return NULL;
	}

	LibertyFont *ret = calloc(1, sizeof(LibertyFont));
	parse_bdf_line(ret, fp);
LOG("BDF: finished reading font\n");
LOG("BDF: read file %s into LibertyFont* %p\n", path, ret);

	return ret;
}

void liberty_free_font(LibertyFont *font)
{
	if (!font) return;
LOG("BDF: freeing font %s\n", font->name);

	if (font->name) free(font->name);
	for (size_t i = 0; i < 128; i++)
	{
LOG("BDF: freeing glyph '%c' %p\n", (char)i, font->glyph[i].points);
		if (font->glyph[i].points)
			free(font->glyph[i].points);
	}
	free(font);
}

SDL_Point liberty_font_char(LibertyFont *font, int glyph, SDL_Point pos)
{
	SDL_Rect viewport;
	if (!font) return pos;
	switch (glyph)
	{
		case ' ':
			pos.x += font->h>>1;
			return pos;
		case '\t':
			pos.x += font->h<<1;
			return pos;
		case '\n': /* TODO: carriage return? not sure how */
			pos.y += font->h;
			return pos;
		default:
			if (!font->glyph[glyph].points) return pos;

			viewport.x = pos.x + font->glyph[glyph].bbx.x;
			viewport.y = pos.y + font->glyph[glyph].bbx.y;
			viewport.w = font->glyph[glyph].bbx.w;
			viewport.h = font->glyph[glyph].bbx.h;
			SDL_SetRenderViewport(Renderer, &viewport);
			// SDL_RenderPoint(Renderer, 1.0f, 1.0f);
			SDL_RenderPoints(Renderer, font->glyph[glyph].points, font->glyph[glyph].count);
			SDL_SetRenderViewport(Renderer, NULL);
			pos.x += font->glyph[glyph].bbx.x + font->glyph[glyph].bbx.w;
			return pos;
	}
}

SDL_Point liberty_font_string(LibertyFont *font, char *string, SDL_Point pos)
{
	for (size_t i = 0; i < strlen(string); i++)
		pos = liberty_font_char(font, string[i], pos);

	return pos;
}

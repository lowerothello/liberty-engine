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

static void draw_hex_bitmap(char *string, SDL_Surface *surface, size_t offset)
{
	long integer = strtol(string, NULL, 16);

	size_t bits = strlen(string)*4; /* every bit */
	for (size_t i = 0; i < bits; i++)
		if (integer&(1<<(bits - (i+1))))
		{ LOG("█"); }
		else
		{ LOG(" "); }

	memcpy(&((char*)surface->pixels)[offset], &integer, strlen(string)>>1);
}

/* returns the glyph width */
/* font->h is expected to be correct */
static size_t parse_bdf_char(LibertyFont *font, SDL_Surface *surface, FILE *fp, size_t offset)
{
	char *keyword;
	char *hexline;
	int index = 0;
	int ret;
	SDL_Rect bbx;

	/* position the file pointer after the startchar line */
	read_file_until_string(fp, "STARTCHAR"); read_file_until_ifs(NULL, fp, "\n");
LOG("BDF: STARTCHAR\n");

	while (1)
	{
		ret = read_file_until_ifs(&keyword, fp, " \n"); /* get the keyword by reading until the first space */

		if (!strcmp(keyword, "ENCODING"))
		{
			read_bdf_number(&index, fp, "\n");
LOG("BDF: ENCODING '%c'\n", index);
		} else if (!strcmp(keyword, "BBX"))
		{
			ret = read_bdf_number(&bbx.w, fp, " \n" );
			ret = read_bdf_number(&bbx.h, fp, " \n" );
			ret = read_bdf_number(&bbx.x, fp, " \n" );
			ret = read_bdf_number(&bbx.y, fp, " \n" );
LOG("BDF: BBX: w=%d h=%d x=%d y=%d\n", bbx.w, bbx.h, bbx.x, bbx.y);
			if (ret != '\n') read_file_until_ifs(NULL, fp, "\n");
		} else if (!strcmp(keyword, "BITMAP"))
		{
LOG("BDF: BITMAP\n");
			if (ret != '\n') read_file_until_ifs(NULL, fp, "\n");
			for (int i = 0; i < bbx.h; i++)
			{
				read_file_until_ifs(&hexline, fp, "\n");
				draw_hex_bitmap(hexline, surface, surface->pitch*i + offset);
LOG("\n");
				free(hexline);
			}
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

	if (index)
	{
		font->glyph[index].bbx = bbx;
		font->glyph[index].offset = offset>>3; /* offset * 8 */
	}
	return (bbx.w>>3) + 1;
}

/* returns the glyph width */
static int get_bdf_char_width(LibertyFont *font, FILE *fp)
{
	int w;

	/* position the file pointer after the startchar line */
	read_file_until_string(fp, "STARTCHAR"); read_file_until_ifs(NULL, fp, "\n");
	read_file_until_string(fp, "BBX ");

	/* only care about width */
	read_bdf_number(&w, fp, " ");

	read_file_until_string(fp, "ENDCHAR"); read_file_until_ifs(NULL, fp, "\n");

	/* round w up to the nearest multiple of 8 */
	return ((w>>3) + 1)<<3;
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
			long fptr = ftell(fp);

			/* get the width of the texture */
			font->w = 0;
			for (i = 0; i < charcount; i++)
				font->w += get_bdf_char_width(font, fp);

			SDL_Surface *surface = SDL_CreateSurface(font->w, font->h, SDL_PIXELFORMAT_INDEX1MSB);
LOG("BDF: created SDL_Surface* %p, with size %d x %d\n", surface, font->w, font->h);

			fseek(fp, fptr, SEEK_SET);

			size_t offset = 0;
			for (i = 0; i < charcount; i++)
				offset += parse_bdf_char(font, surface, fp, offset);
			font->texture = SDL_CreateTextureFromSurface(Renderer, surface);
			if (!font->texture) SDL_ERR();
			uint32_t format = 0;
			int ret = SDL_QueryTexture(font->texture, &format, NULL, NULL, NULL);
			if (!ret) SDL_ERR();
LOG("BDF: created SDL_Texture with format 0x%x\n", format);
			SDL_DestroySurface(surface); surface = NULL;
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
LOG("BDF: opening file %s\n", path);
	FILE *fp = fopen(path, "r");
	if (!fp)
	{
LOG("file %s doesn't exist\n", path);
LOG("BDF: failed to read file\n");
		return NULL;
	}

	LibertyFont *ret = malloc(sizeof(LibertyFont));
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
	// if (font->texture) SDL_DestroyTexture(font->texture);
	free(font);
}

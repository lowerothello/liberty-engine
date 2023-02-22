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
			font->glyph[index].points = malloc(sizeof(LibertyVec2) * font->glyph[index].bbx.w * font->glyph[index].bbx.h);
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
			font->glyph[index].points = realloc(font->glyph[index].points, sizeof(LibertyVec2) * font->glyph[index].count);
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
		} else if (ret != '\n' && read_file_until_ifs(NULL, fp, "\n") == EOF)
		{
			free(keyword);
			return;
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
LOG("BDF: failed to open font, file '%s' can't be opened\n", path);
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
LOG("BDF: freeing font %s %p\n", font->name, font);

	if (font->name) free(font->name);
	for (size_t i = 0; i < 128; i++)
	{
LOG("BDF: freeing glyph '%c' %p\n", (char)i, font->glyph[i].points);
		if (font->glyph[i].points)
			free(font->glyph[i].points);
	}
	free(font);
}

LibertyVec2 draw_font_char(LibertyFont *font, int glyph, LibertyVec2 pos, int cret, bool draw)
{
	if (!font)
	{
LOG("using uninitialized LibertyFont!\n");
		return pos;
	}

	if (draw && font->glyph[glyph].points)
		liberty_draw_pixels(
				font->glyph[glyph].points,
				font->glyph[glyph].count,
				(LibertyVec2){pos.x + font->glyph[glyph].bbx.x, pos.y + font->glyph[glyph].bbx.y});

	pos.x += font->glyph[glyph].bbx.x + font->glyph[glyph].bbx.w;

	switch (glyph)
	{
		case ' ':
			pos.x += font->h>>1;
			break;
		case '\t':
			pos.x += font->h<<1;
			break;
		case '\n':
			pos.y += font->h;
			pos.x = cret;
			break;
		case '\r':
			pos.x = cret;
			break;
	}

	return pos;
}

LibertyVec2 liberty_draw_font_string(LibertyFont *font, LibertyVec2 pos, char *string)
{
	if (!font)
	{
LOG("using uninitialized LibertyFont!\n");
		return pos;
	}

	int cret = pos.x;
	for (size_t i = 0; i < strlen(string); i++)
		pos = draw_font_char(font, string[i], pos, cret, 1);

	return pos;
}

LibertyVec2 liberty_draw_font_string_count(LibertyFont *font, LibertyVec2 pos, char *string, size_t count)
{
	count = MIN(count, strlen(string)); /* don't try to write out of range */
	char *trimmedstring = strdup(string);
	trimmedstring[count+1] = '\0';
	LibertyVec2 ret = liberty_draw_font_string(font, pos, trimmedstring);
	free(trimmedstring);
	return ret;
}

void liberty_draw_font_animation(LibertyFont *font, LibertyVec2 pos, float rate)
{
	if (!font)
	{
LOG("using uninitialized LibertyFont!\n");
		return;
	}

LOG("anim: %p, %f\n", font, font->frame);

	short passed = -1;
	for (uint8_t i = 0; i < 128; i++)
	{
		if (font->glyph[i].points)
		{
LOG("%d populated\n", i);
			passed++;
		}

		if (passed == (int)font->frame)
		{
			draw_font_char(font, i, pos, 0, 1);
			font->frame += rate;
LOG("%d, %f\n", i, font->frame);
			return;
		}
	}
	font->frame = 0.0f;
}

LibertyRect liberty_get_font_string_bbx(LibertyFont *font, LibertyVec2 pos, char *string)
{
	LibertyRect ret;
	ret.x = pos.x;
	ret.y = pos.y;
	ret.w = ret.h = 0.0f;

	int cret = pos.x;
	for (size_t i = 0; i < strlen(string); i++)
	{
		pos = draw_font_char(font, string[i], pos, cret, 0); /* doesn't draw */
		ret.w = MAX(ret.w, pos.x - ret.x);
		ret.h = MAX(ret.h, pos.y - ret.y);
	}
	ret.h += font->h;

	return ret;
}

LibertyVec2 liberty_draw_font_string_outline(LibertyFont *font, LibertyVec2 pos, char *string)
{
	LibertyRect rect = liberty_get_font_string_bbx(font, pos, string);
	rect.x -= 1;
	rect.y -= 1;
	rect.w += 3;
	rect.h += 2;
	liberty_draw_rect(0, rect);
	return liberty_draw_font_string(font, pos, string);
}
LibertyVec2 liberty_draw_font_string_reverse(LibertyFont *font, LibertyVec2 pos, char *string)
{
	LibertyRect rect = liberty_get_font_string_bbx(font, pos, string);
	rect.x -= 1;
	rect.y -= 1;
	rect.w += 3;
	rect.h += 2;
	liberty_draw_rect(1, rect);

	LibertyRGB oldcolour = liberty_get_colour();
	liberty_set_colour((LibertyRGB){0x00, 0x00, 0x00, 0x00});
	LibertyVec2 ret = liberty_draw_font_string(font, pos, string);
	liberty_set_colour(oldcolour);

	return ret;
}

LibertyVec2 liberty_get_font_string_centre(LibertyFont *font, LibertyRect rect, char *string)
{
	LibertyRect bbx = liberty_get_font_string_bbx(font, (LibertyVec2){0, 0}, string);
	return (LibertyVec2){rect.x + ((rect.w - bbx.w)*0.5f), rect.y + ((rect.h - bbx.h)*0.5f)};
}

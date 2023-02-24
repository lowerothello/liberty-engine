LibertyGridMap *liberty_new_grid_map(uint16_t width, uint16_t height)
{
	LibertyGridMap *ret = calloc(1, sizeof(LibertyGridMap) + sizeof(LibertyGridCell)*width*height);
	ret->width = width;
	ret->height = height;
	return ret;
}

LibertyGridMap *liberty_new_grid_map_from_file(char *path)
{
	FILE *fp = fopen(path, "r");

	int input;
	uint16_t width;
	uint16_t x = 0, y = 0;
	LibertyGridCell **cell = calloc(1, sizeof(LibertyGridCell*));
	cell[0] = NULL;
	while ((input = fgetc(fp)) != -1)
	{
		if (input == '\n')
		{
			x = 0;
			y++;
			cell = realloc(cell, (y+1) * sizeof(LibertyGridCell*));
			cell[y] = NULL;
			continue;
		}
		x++;
		width = MAX(x, width);

		cell[y] = realloc(cell[y], sizeof(LibertyGridCell) * width);

		/* questionable ascii-foo, i want hex numbers and ' ' to also mean 0 */
		if      (input >= 'a') cell[y][x - 1].id = input - 'a' + 0xa;
		else if (input >= 'A') cell[y][x - 1].id = input - 'A' + 0xa;
		else if (input >= '0') cell[y][x - 1].id = input - '0';
		else if (input >= ' ') cell[y][x - 1].id = input - ' ';
	}

	LibertyGridMap *ret = liberty_new_grid_map(width, y);
	for (uint16_t yy = 0; yy < y; yy++)
	{
		memcpy(&ret->cell[ret->width * yy], cell[yy], sizeof(LibertyGridCell) * ret->width);
		free(cell[yy]);
	}

	free(cell);
	return ret;
}

LibertyGridCell *liberty_get_grid_map_cell(LibertyGridMap *map, uint16_t x, uint16_t y)
{
	return &map->cell[y*map->width + x];
}

void liberty_free_grid_map(LibertyGridMap *map)
{
	free(map);
}

size_t liberty_get_grid_map_all_of_id(LibertyGridMap *map, LibertyVec2 **offsets, uint32_t id)
{
	size_t offsetc = 0;

	*offsets = malloc(sizeof(LibertyVec2) * map->width * map->height);

	for (uint16_t y = 0; y < map->height; y++)
		for (uint16_t x = 0; x < map->width; x++)
			if (liberty_get_grid_map_cell(map, x, y)->id == id)
			{
				(*offsets)[offsetc] = (LibertyVec2){x, y};
				offsetc++;
			}

	*offsets = realloc(*offsets, sizeof(LibertyVec2) * offsetc);
	return offsetc;
}

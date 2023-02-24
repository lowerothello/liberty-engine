/* grid based map system */

typedef struct LibertyGridCell
{
	uint32_t id;
} LibertyGridCell;

typedef struct LibertyGridMap
{
	uint16_t        width;
	uint16_t        height;
	LibertyGridCell cell[];
} LibertyGridMap;

LibertyGridMap *liberty_new_grid_map(uint16_t width, uint16_t height);
LibertyGridMap *liberty_new_grid_map_from_file(char *path);
LibertyGridCell *liberty_get_grid_map_cell(LibertyGridMap *map, uint16_t x, uint16_t y);
void liberty_free_grid_map(LibertyGridMap *map);

/* TODO */
/* allocates and populates *offsets */
/* returns how many entries are in *offsets */
/* *offsets must be free'd after use */
size_t liberty_get_grid_map_all_of_id(LibertyGridMap *map, LibertyVec2 **offsets, uint32_t id);

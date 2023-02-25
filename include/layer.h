/* not intended to be accessed directly */
/* a void pointer so it can be a direct wrapper over (SDL_Texture) lol */
typedef struct LibertyLayer
{
	uint16_t width;
	uint16_t height;
	void    *data;
} LibertyLayer;

/* allocate/free layers */
LibertyLayer *liberty_new_layer(LibertyVec2 size);
void liberty_free_layer(LibertyLayer *layer);

/* set the active layer for drawing operations */
/* pass NULL to draw directly to the screen */
void liberty_set_draw_layer(LibertyLayer *layer);

/* blit a layer to the active layer */
/* .offset is the position the top left corner will be drawn to */
void liberty_draw_layer(
		LibertyLayer *layer,
		SDL_BlendMode blend, /* TODO: shouldn't be an sdl enum */
		LibertyVec2 offset);

/* blit a single row of a layer to the active layer */
/* .row is the row to draw, the destination y position takes into account the row */
/* .offset is the position the top left corner of the layer would be */
void liberty_draw_layer_row(
		LibertyLayer *layer,
		uint16_t row,
		SDL_BlendMode blend, /* TODO: shouldn't be an sdl enum */
		LibertyVec2 offset, float scale);

/* blit a single column of a layer to the active layer */
/* .column is the column to draw, the destination y position takes into account the row */
/* .offset is the position the top left corner of the layer would be */
void liberty_draw_layer_column(
		LibertyLayer *layer,
		uint16_t column,
		SDL_BlendMode blend, /* TODO: shouldn't be an sdl enum */
		LibertyVec2 offset, float scale);

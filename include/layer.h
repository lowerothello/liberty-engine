/* not intended to be accessed directly */
/* a void pointer so it can be a direct wrapper over (SDL_Texture) lol */
typedef void LibertyLayer; /* private pointer */

/* allocate/free layers */
LibertyLayer *liberty_new_layer(LibertyVec2 size);
void liberty_free_layer(LibertyLayer *layer);

/* set the active layer for drawing operations */
/* pass NULL to draw directly to the screen */
void liberty_set_draw_layer(LibertyLayer *layer);

/* blit a layer to the active layer */
/* .offset is the position the top left corner will be drawn to */
/* .tint is a multiplier for each colour channel */
void liberty_draw_layer(LibertyLayer *layer,
		LibertyVec2 offset, LibertyColour tint);

/* blit a single row of a layer to the active layer */
/* .row is the row to draw, the destination y position takes into account the row */
/* .offset is the position the top left corner of the layer would be */
/* .tint is a multiplier for each colour channel */
void liberty_draw_layer_row(LibertyLayer *layer, uint16_t row,
		LibertyVec2 offset, LibertyColour tint);

typedef void LibertyLayer; /* private pointer */

LibertyLayer *liberty_new_layer(LibertyVec2 size);
void liberty_free_layer(LibertyLayer *layer);

/* set to NULL to draw directly to the screen */
void liberty_set_draw_layer(LibertyLayer *layer);

void liberty_draw_layer(LibertyLayer *layer,
		LibertyVec2 offset, LibertyColour tint);

void liberty_draw_layer_row(LibertyLayer *layer, uint16_t row,
		LibertyVec2 offset, LibertyColour tint);

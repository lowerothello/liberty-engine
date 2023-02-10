typedef void LibertyLayer; /* private pointer */

LibertyLayer *liberty_new_layer(void);
void liberty_free_layer(LibertyLayer *layer);

/* set to NULL to draw directly to the screen */
void liberty_set_draw_layer(LibertyLayer *layer);

/* dump a layer to the screen */
void liberty_draw_layer(LibertyLayer *layer,
		SDL_Point offset, LibertyColour tint);

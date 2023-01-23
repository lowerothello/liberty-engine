/*
 * configuration options for the window.
 * initial config is returned from liberty_callback_init.
 * config can be changed at any time with liberty_change_config.
 */
typedef struct LibertyConfig {
	const char *title;
	uint8_t scale;
	uint16_t width, height;
} LibertyConfig;

/* fetches the current config */
LibertyConfig liberty_get_config(void);

/* sets a new config */
void liberty_set_config(LibertyConfig);

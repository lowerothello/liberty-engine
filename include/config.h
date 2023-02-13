/*
 * configuration options for the window.
 * config can be changed at any time with liberty_set_config.
 */
typedef struct LibertyConfig
{
	const char *title;
	uint16_t    width;
	uint16_t    height;
	uint8_t     scale;
	char        fullscreen; /* 0=windowed; SDL_WINDOW_FULLSCREEN=fullscreen; SDL_WINDOW_FULLSCREEN_DESKTOP=borderless */
	bool        vsync;
	float       framerate; /* target framerate for draw calls, < 1.0f for unlimited */
} LibertyConfig;

/* fetches the current config */
LibertyConfig liberty_get_config(void);

/* sets a new config */
void liberty_set_config(LibertyConfig);

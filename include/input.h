typedef struct {
	SDL_Keycode key;
	void      (*press_callback  )(void);
	void      (*release_callback)(void);
} LibertyInputEntry;

typedef struct {
	uint32_t           count;
	LibertyInputEntry *entry;
} LibertyInputState;




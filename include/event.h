typedef enum {
	LIBERTY_EVENT_NULL = 0, /* unused */
	LIBERTY_EVENT_KEY,
} LibertyEventType;

typedef struct {
	LibertyEventType type;
	uint8_t          release; /* true if the key was released */
	uint8_t          repeat;  /* true if this is a key repeat */
	SDL_Keycode      keycode;
} LibertyKey;

/* main event enum */
typedef union {
	LibertyEventType type;
	LibertyKey       key;
	/* TODO: padding */
} LibertyEvent;

typedef enum {
	LIBERTY_EVENT_NULL = 0, /* unused */
	/* LibertyKey */
	LIBERTY_EVENT_KEYPRESS,
	LIBERTY_EVENT_KEYRELEASE,
	LIBERTY_EVENT_KEYREPEAT,
} LibertyEventType;

typedef struct {
	LibertyEventType type;
	SDL_Keycode      keycode;
} LibertyKey;

/* main event enum */
typedef union {
	LibertyEventType type;
	LibertyKey       key;
	/* TODO: padding */
} LibertyEvent;

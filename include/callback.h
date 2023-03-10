/*
 * LIBERTY CALLBACKS
 *   These are defined as stubs by liberty, and
 *   are expected to be redefined by the game.
 */

/* callback return value */
typedef enum
{
	LIBERTY_SIGNAL_OK = 0, /* nothing to do          */
	LIBERTY_SIGNAL_TERM,   /* close liberty          */
	LIBERTY_SIGNAL_UPDATE, /* trigger an update call */
} LibertySignal;


/* config the window is initialized with */
LibertyConfig liberty_default_config;

/* called just after the window has been initialized, should initialize the game state. */
void liberty_callback_init(void);

/* called at the very end, should cleanup after the game. */
void liberty_callback_cleanup(void);

/* called as often as possible.
 * .deltatime is how many seconds have passed since the last update call.
 * returns a LibertySignal to ask the main thread to do things. */
LibertySignal liberty_callback_update(double deltatime);

/* called when a new frame should be drawn.
 * .deltatime is how many seconds have passed since the last draw call. */
void liberty_callback_draw(double deltatime);

/* called when an event is recieved
 * returns a LibertySignal to ask the main thread to do things.
 * parses raw SDL events cos there's no reason to add an abstraction layer. */
LibertySignal liberty_callback_event(SDL_Event event);

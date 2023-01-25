/*
 * LIBERTY CALLBACKS
 *   These are defined as stubs by liberty, and
 *   are expected to be redefined by the game.
 *   You probably want to redefine at least:
 *    - liberty_callback_init
 *    - liberty_callback_update
 *    - liberty_callback_draw
 */

/*
 * called before the window is initialized, should initialize the game's state.
 * returns a LibertyConfig struct that describes the window that should be initialized.
 */
LibertyConfig liberty_callback_init(void);

/*
 * called at the very end, should cleanup after the game.
 */
void liberty_callback_cleanup(void);

/*
 * called as often as possible.
 * .deltatime is how many seconds have passed since the last update call.
 * returns a LibertySignal to ask the main thread to do things.
 */
LibertySignal liberty_callback_update(double deltatime);

/*
 * called when a new frame should be drawn.
 * .renderer is the SDL_Renderer that should be drawn to.
 */
void liberty_callback_draw(SDL_Renderer *renderer);

/*
 * called when an event is recieved
 * returns a LibertySignal to ask the main thread to do things.
 * parses raw SDL events cos there's no reason to add an abstraction layer.
 */
LibertySignal liberty_callback_event(SDL_Event event);

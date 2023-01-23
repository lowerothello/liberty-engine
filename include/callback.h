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
 * called at the very beginning, should initialize the game.
 */
void liberty_callback_init(void);

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
 */
void liberty_callback_draw(void);

/*
 * called when an event is recieved
 * returns a LibertySignal to ask the main thread to do things.
 */
LibertySignal liberty_callback_event(LibertyEvent event);

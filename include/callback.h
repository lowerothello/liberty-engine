/*
 * LIBERTY CALLBACKS
 *   These are defined as stubs by liberty, and
 *   are expected to be redefined by the game.
 *   You probably want to redefine at least:
 *    - liberty_callback_update
 *    - liberty_callback_draw
 */

/*
 * should initialize the window.
 * expected to return a Liberty* returned
 * from liberty_init().
 */
Liberty *liberty_callback_init(void);


enum LibertySignal {
	LIBERTY_SIGNAL_OK = 0, /* nothing to do */
	LIBERTY_SIGNAL_TERM,   /* close liberty */
};
/*
 * called as often as possible.
 * .ticktime is how many milliseconds have elapsed since liberty was initialized.
 * returns a LibertySignal enum to ask the main thread to do things.
 */
enum LibertySignal liberty_callback_update(Liberty *l, uint64_t tick);

/*
 * called when a new frame should be drawn.
 */
void liberty_callback_draw(Liberty *l);

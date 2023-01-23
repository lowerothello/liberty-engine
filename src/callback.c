/* stub callback definitions */
// #define LIBERTY_DEFAULT_TITLE "Liberty"
// #define LIBERTY_DEFAULT_SCALE 1
// #define LIBERTY_DEFAULT_WIDTH 640
// #define LIBERTY_DEFAULT_HEIGHT 480
// #define LIBERTY_DEFAULT_FRAMERATE 0
// return liberty_init(
// 		LIBERTY_DEFAULT_TITLE,
// 		LIBERTY_DEFAULT_SCALE,
// 		LIBERTY_DEFAULT_WIDTH,
// 		LIBERTY_DEFAULT_HEIGHT,
// 		LIBERTY_DEFAULT_FRAMERATE);
void liberty_callback_init(void) { }
void liberty_callback_cleanup(void) { }

LibertySignal liberty_callback_update(double deltatime)
{ return LIBERTY_SIGNAL_OK; }

void liberty_callback_draw(void)
{ }

LibertySignal liberty_callback_event(LibertyEvent event)
{ return LIBERTY_SIGNAL_OK; }

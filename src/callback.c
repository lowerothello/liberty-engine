/* stub callback definitions */
#define LIBERTY_DEFAULT_TITLE "Liberty"
#define LIBERTY_DEFAULT_SCALE 4
#define LIBERTY_DEFAULT_WIDTH 640
#define LIBERTY_DEFAULT_HEIGHT 480
#define LIBERTY_DEFAULT_FRAMERATE 0
Liberty *liberty_callback_init(void)
{
	return liberty_init(
			LIBERTY_DEFAULT_TITLE,
			LIBERTY_DEFAULT_SCALE,
			LIBERTY_DEFAULT_WIDTH,
			LIBERTY_DEFAULT_HEIGHT,
			LIBERTY_DEFAULT_FRAMERATE);
}

enum LibertySignal liberty_callback_update(Liberty *l, uint64_t tick)
{ return LIBERTY_SIGNAL_OK; }

void liberty_callback_draw(Liberty *l)
{ return; }


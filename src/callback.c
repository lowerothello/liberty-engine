/* stub callback definitions */
#define LIBERTY_DEFAULT_TITLE "Liberty"
#define LIBERTY_DEFAULT_SCALE 1
#define LIBERTY_DEFAULT_WIDTH 640
#define LIBERTY_DEFAULT_HEIGHT 480

LibertyConfig liberty_callback_init(void)
{
	LibertyConfig ret;
	ret.title = LIBERTY_DEFAULT_TITLE;
	ret.scale = LIBERTY_DEFAULT_SCALE;
	ret.width = LIBERTY_DEFAULT_WIDTH;
	ret.height = LIBERTY_DEFAULT_HEIGHT;
	return ret;
}

void liberty_callback_cleanup(void) { }

LibertySignal liberty_callback_update(double deltatime)
{ return LIBERTY_SIGNAL_OK; }

void liberty_callback_draw(void)
{ }

/* more sophisticated stub handler as an example */
LibertySignal liberty_callback_event(SDL_Event event)
{
	switch (event.type)
	{
		case SDL_KEYDOWN: /* fall through */
		case SDL_KEYUP:
LOG("key %s %s\n", SDL_GetKeyName(event.key.keysym.sym), event.key.repeat ? "repeated" : (event.type == SDL_KEYUP ? "released" : "pressed"));
			break;
	}
	return LIBERTY_SIGNAL_OK;
}

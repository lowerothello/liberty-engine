/* stub callback definitions */

LibertyConfig liberty_default_config =
{
	.title = "Liberty",
	.width = 640,
	.height = 480,
	.scale = 1,
	.fullscreen = 0,
	.vsync = 0,
	.framerate = 0.0f,
};

void liberty_callback_init(void) { }

void liberty_callback_cleanup(void) { }

LibertySignal liberty_callback_update(double deltatime)
{ return LIBERTY_SIGNAL_OK; }

void liberty_callback_draw(double deltatime) { }

/* more sophisticated stub handler as an example */
LibertySignal liberty_callback_event(SDL_Event event)
{
	switch (event.type)
	{
		case SDL_EVENT_KEY_DOWN: /* fall through */
		case SDL_EVENT_KEY_UP:
LOG("key %s %s\n", SDL_GetKeyName(event.key.keysym.sym), event.key.repeat ? "repeated" : ((event.type == SDL_EVENT_KEY_UP) ? "released" : "pressed"));
			break;
	}
	return LIBERTY_SIGNAL_OK;
}

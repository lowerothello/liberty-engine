/* stub callback definitions */

LibertyConfig liberty_callback_init(void)
{
	LibertyConfig ret;
	ret.title = "Liberty";
	ret.scale = 1;
	ret.width = 640;
	ret.height = 480;
	ret.framerate = 60.0f;
	ret.fullscreen = 0;
	ret.vsync = 0;
	return ret;
}

void liberty_callback_cleanup(void) { }

LibertySignal liberty_callback_update(double deltatime)
{ return LIBERTY_SIGNAL_OK; }

void liberty_callback_draw(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0xff,0xff,0xff,0xff);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

/* more sophisticated stub handler as an example */
LibertySignal liberty_callback_event(SDL_Event event)
{
	switch (event.type)
	{
		case SDL_EVENT_KEY_DOWN: /* fall through */
		case SDL_EVENT_KEY_UP:
LOG("key %s %s\n", SDL_GetKeyName(event.key.keysym.sym), event.key.repeat ? "repeated" : (event.type == SDL_EVENT_KEY_UP ? "released" : "pressed"));
			break;
	}
	return LIBERTY_SIGNAL_OK;
}

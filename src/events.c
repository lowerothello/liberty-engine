void privliberty_eventhandler(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					case SDL_WINDOWEVENT_CLOSE: privliberty_cleanup(0);
				} break;
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
				break;
		}
	}
}

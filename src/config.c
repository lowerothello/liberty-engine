/* public */

LibertyConfig liberty_get_config(void)
{ return Config; }

void liberty_set_config(LibertyConfig config)
{
	SDL_Event *event = calloc(1, sizeof(SDL_Event));
	event->type = LIBERTY_EVENT;
	event->user.code = LIBERTY_EVENT_CONFIG;
	event->user.data1 = malloc(sizeof(LibertyConfig));
	memcpy(event->user.data1, &config, sizeof(LibertyConfig));
	SDL_PushEvent(event);
	free(event);
}


/* private */

/* reapplies Config where it is relevant */
void reapply_config(void)
{
	resize_window(Window, Config);
}

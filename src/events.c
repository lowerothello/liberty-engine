typedef enum LibertyEvent
{
	LIBERTY_EVENT_CONFIG,
} LibertyEvent;

uint32_t LIBERTY_EVENT;
void register_sdl_user_events(void)
{
	LIBERTY_EVENT = SDL_RegisterEvents(1);
}

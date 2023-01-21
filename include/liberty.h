/*
 * LIBERTY GAME ENGINE
 */

#include <stdint.h>
#include <SDL.h>


typedef struct {
 	SDL_Keycode key;
	void (*keydown_callback)(void);
	void (*keyup_callback)(void);
} LibertyKeybindEntry;

typedef struct {
	uint32_t count;
	LibertyKeybindEntry *entry;
} LibertyKeybindState;

/* TODO: proper private struct or smth */
typedef struct {
	uint8_t           scale;
	uint16_t          width;
	uint16_t          height;
	uint16_t          framerate;
	float             frametime; /* calculated from .framerate */
	float             frametimeelapsed; /* internal */
	SDL_Window       *window;
	LibertyKeybindState keybinds;
} Liberty;

#include "events.h" /* events shit */
#include "init.h" /* initialization and cleanup */
#include "callback.h" /* engine callbacks */
#include "draw.h"

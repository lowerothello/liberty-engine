/*
 * LIBERTY GAME ENGINE
 */

#include <stdint.h>
#include <SDL.h>

/* TODO: proper private struct or smth */
typedef struct {
	uint8_t     scale;
	uint16_t    width;
	uint16_t    height;
	uint16_t    framerate;
	float       frametime; /* calculated from .framerate */
	float       frametimeelapsed; /* internal */
	SDL_Window *window;
} Liberty;

#include "init.h" /* initialization and cleanup */
#include "callback.h" /* engine callbacks */
#include "draw.h"

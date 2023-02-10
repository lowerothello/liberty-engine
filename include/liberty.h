/*
 * LIBERTY GAME ENGINE
 */

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

/*
 * working with liberty:
 *
 * don't define a main function, cos liberty defines one internally.
 * instead, define the callbacks described in "callback.h" to hook in your own
 * code.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>

/* public headers */
#include "config.h"
#include "signal.h"   /* required by "callback.h" */
#include "callback.h" /* engine callbacks */
#include "colour.h"   /* colours */
#include "draw.h"     /* draw code */
#include "font.h"
#include "layer.h"

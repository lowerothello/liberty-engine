/*
 * LIBERTY GAME ENGINE
 */


/*
 * working with liberty:
 *
 * don't define a main function, cos liberty defines one internally.
 * instead, define the callbacks described in "callback.h" to hook in your own
 * code.
 */

#include <stdint.h>
#include <SDL.h>

/* public headers */
#include "config.h"
// #include "event.h"    /* required by "callback.h" */
#include "signal.h"   /* required by "callback.h" */
#include "callback.h" /* engine callbacks */
#include "draw.h"

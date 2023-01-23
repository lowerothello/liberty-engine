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


#include "callback.h" /* engine callbacks */
#include "draw.h"
#include "input.h"

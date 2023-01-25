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

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>

/* public headers */
#include "config.h"
#include "signal.h"   /* required by "callback.h" */
#include "callback.h" /* engine callbacks */
#include "font.h"

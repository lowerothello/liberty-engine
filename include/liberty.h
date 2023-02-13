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
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SDL.h>

/* public headers */
#include "maths.h"
#include "config.h"
#include "callback.h"
#include "draw.h"
#include "font.h"
#include "dialogue.h"
#include "layer.h"

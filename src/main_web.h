#ifndef MAIN_WEB_H
#define MAIN_WEB_H

#define PLATFORM_WEB

#include <emscripten.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Headers
#include "assets.h"
#include "utility.h"
#include "logger.h"
#include "ftimer.h"
#include "render.h"
#include "window.h"
#include "input.h"
#include "audio.h"
#include "settings.h"
#include "score.h"
#include "player.h"
#include "effect.h"
#include "entity.h"
#include "application.h"

// Sources
#include "utility.c"
#include "logger.c"
#include "ftimer.c"
#include "render.c"
#include "window.c"
#include "input.c"
#include "audio.c"
#include "settings.c"
#include "score.c"
#include "player.c"
#include "effect.c"
#include "entity.c"
#include "application.c"

#endif /* MAIN_WEB_H */

#ifndef _GAME_H_
#define _GAME_H_

#include <SDL.h>
#include "sprite.h"


// buttons
typedef struct {
	sprite* play;
} game_buttons;

extern game_buttons* b;


// game objects
typedef struct {
	sprite* bg1;
	sprite* bg2;
} game_environment;

extern game_environment* env;


void game_init();
void game_draw();
void game_free();
int game_handleEvent(SDL_Event* e);
#endif

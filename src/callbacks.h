#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_

#include <SDL.h>
#include "sprite.h"
#include "game.h"
#include "tween.h"
#include "easing.h"

void callback_startGameFromMenu(void* s)
{
	// do smth
}

void callback_buttonUpDefault(void* s)
{
	sprite* sp = (sprite*)s;

	sp->currentFrame = 1;
	tween_create(sp, sp->x, sp->y, 1, 1, 0, 50, &elasticOut);
}


void callback_buttonDownDefault(void* s)
{
	sprite* sp = (sprite*)s;
	
	sp->currentFrame = 2;
	tween_create(sp, sp->x, sp->y, 1.5, 1.5, 15, 50, &elasticOut);
}

#endif

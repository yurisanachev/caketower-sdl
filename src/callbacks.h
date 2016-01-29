#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_

#include <SDL.h>
#include "sprite.h"
#include "game.h"
#include "tween.h"
#include "easing.h"

void callback_tweenDown(entity* sender);

void callback_tweenUp(entity* sender)
{
	tween_create(sender, sender->x, sender->y + 5, 1.2, 1, 0, 500, 0, &backIn, &callback_tweenDown);	
}

void callback_tweenDown(entity* sender)
{
	tween_create(sender, sender->x, sender->y - 5, 1, 1, 0, 500, 0, &backOut, &callback_tweenUp);	
}

void callback_startGameFromMenu(entity* sender)
{
	// do smth
	tween_killTweensOf(sender);
	game_hideMenu();
	game_showGame();
}

void callback_buttonUpDefault(entity* sender)
{
	sprite* sp = (sprite*)sender;

	sp->currentFrame = 1;
	//tween_create(sp, sp->x, sp->y, 1, 1, 0, 500, 0, &elasticOut, NULL);
}


void callback_buttonDownDefault(entity* sender)
{
	sprite* sp = (sprite*)sender;
	
	sp->currentFrame = 2;
	//tween_create(sp, sp->x, sp->y, 1.5, 1.5, 15, 500, 0, &elasticOut, NULL);
}

#endif

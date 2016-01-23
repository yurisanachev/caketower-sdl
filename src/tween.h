#ifndef _TWEEN_H_
#define _TWEEN_H_

#include <SDL.h>
#include "sprite.h"
#include "list.h"

typedef struct {
	int dx;
	int dy;
	double dScaleX;
	double dScaleY;
	double dRotation;
	int sx;
	int sy;
	double sScaleX;
	double sScaleY;
	double sRotation;
	int time;
	int elapsed;
	sprite* obj;
} tween;

extern list_t* tweens;

void tween_create(sprite* obj,
		int dx, 
		int dy, 
		double dSX, 
		double dSY, 
		double dR, 
		int time);

void tween_update(tween* t);
void tween_destroy(tween* t);
#endif

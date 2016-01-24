#include "tween.h"

list_t* tweens = NULL;

void tween_create(sprite* obj,
		int dx, 
		int dy, 
		double dSX, 
		double dSY, 
		double dR,
		int time,
		double (*easing)(double))
{
	tween* t = (tween*)malloc(sizeof(tween));	
	
	t->sx = obj->x;
	t->sy = obj->y;
	t->sScaleX = obj->scaleX;
	t->sScaleY = obj->scaleY;
	t->sRotation = obj->rotation;
	
	t->dx = dx - t->sx;
	t->dy = dy - t->sy;
	t->dScaleX = dSX - t->sScaleX;
	t->dScaleY = dSY - t->sScaleY;
	t->dRotation = dR - t->sRotation;
	

	t->time = time;
	t->elapsed = 0;
	t->obj = obj;
	
	t->easing = NULL;
	t->easing = easing;	

	list_add_back(&tweens, (void*)t);
}

void tween_update(tween* t)
{
	if (t->obj == NULL || t->elapsed == t->time)
	{
		list_remove(&tweens, t);
		tween_destroy(t);

		return;
	}
	
	t->elapsed++;
	
	double delta = 1.0 * t->elapsed / t->time;
	
	if (t->easing != NULL) delta = t->easing(delta);
	
	sprite_setPosition(t->obj, t->sx + delta * t->dx, t->sy + delta * t->dy);	
	t->obj->scaleX = t->sScaleX + delta * t->dScaleX; 
	t->obj->scaleY = t->sScaleY + delta * t->dScaleY; 
	t->obj->rotation = t->sRotation + delta * t->dRotation; 
	
}

void tween_destroy(tween* t)
{
	t->obj = NULL;
	free(t);
}

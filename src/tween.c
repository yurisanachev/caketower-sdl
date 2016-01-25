#include "tween.h"

list_t* tweens = NULL;

void tween_create(sprite* obj,
		int dx, 
		int dy, 
		double dSX, 
		double dSY, 
		double dR,
		int time,
		int delay,
		double (*easing)(double),
		void (*onComplete)())
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
	
	// magic 0.9
	t->delay = delay / (1000 / 60) * 0.9;
	t->time = time / (1000 / 60) * 0.9;
	t->elapsed = 0;
	t->obj = obj;
	
	t->easing = NULL;
	t->easing = easing;	

	t->onComplete = onComplete;

	list_add_back(&tweens, (void*)t);
}

void tween_killTweensOf(void* data)
{
	list_t* it = tweens;
	
	while (it)
	{
		list_t* safe = it->next;
		tween* curr = (tween*)(it->value);
		if (curr->obj == data) tween_destroy(curr); 
		it = safe;		
	}
}

void tween_update(tween* t)
{
	if (t->obj == NULL || t->elapsed == t->time)
	{
		if (t->onComplete != NULL) t->onComplete(); 
	
		tween_destroy(t);

		return;
	}
	
	if (t->delay > 0) 
	{
		t->delay--;
	} else {
		t->elapsed++;	
		
		double delta = 1.0 * t->elapsed / t->time;
		
		if (t->easing != NULL) delta = t->easing(delta);
		
		sprite_setPosition(t->obj, t->sx + delta * t->dx, t->sy + delta * t->dy);	
		t->obj->scaleX = t->sScaleX + delta * t->dScaleX; 
		t->obj->scaleY = t->sScaleY + delta * t->dScaleY; 
		t->obj->rotation = t->sRotation + delta * t->dRotation; 
	}
}

void tween_destroy(tween* t)
{
	list_remove(&tweens, t);
	
	t->obj = NULL;
	t->easing = NULL;
	t->onComplete = NULL;

	free(t);
}

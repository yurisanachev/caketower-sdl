#include "tween.h"

list_t* tweens = NULL;

void tween_create(entity* ent,
		int dx, 
		int dy, 
		double dSX, 
		double dSY, 
		double dR,
		int time,
		int delay,
		double (*easing)(double),
		void (*onComplete)(entity*))
{
	tween* t = (tween*)malloc(sizeof(tween));	

	// figure out what object we are tweening
	
	t->sx = ent->x;
	t->sy = ent->y;
	t->dx = dx - t->sx;
	t->dy = dy - t->sy;
	
	if (ent->t == SPRITE)
	{
		sprite* s = (sprite*)(ent);
		t->sScaleX = s->scaleX;
		t->sScaleY = s->scaleY;
		t->sRotation = s->rotation;
	
		t->dScaleX = dSX - t->sScaleX;
		t->dScaleY = dSY - t->sScaleY;
		t->dRotation = dR - t->sRotation;
	}
	
	// magic 0.9 :)
	t->delay = delay / (1000 / 60) * 0.9;
	t->time = time / (1000 / 60) * 0.9;
	t->elapsed = 0;
	t->obj = ent;
	
	t->easing = NULL;
	t->easing = easing;	

	t->onComplete = onComplete;

	list_add_back(&tweens, (void*)t);
}

void tween_killTweensOf(entity* data)
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
		if (t->onComplete != NULL) t->onComplete(t->obj); 
	
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
	
		engine_setEntityPosition(t->obj, t->sx + delta * t->dx, t->sy + delta * t->dy);	
			
		if (t->obj->t == SPRITE) 
		{
			sprite* s = (sprite*)(t->obj);

			s->scaleX = t->sScaleX + delta * t->dScaleX; 
			s->scaleY = t->sScaleY + delta * t->dScaleY; 
			s->rotation = t->sRotation + delta * t->dRotation; 
		} 
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

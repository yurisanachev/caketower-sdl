#include "sprite.h"
#include "assets.h"
#include <stdio.h>
#include "engine.h"

void sprite_free(sprite* s)
{
    s->tex = NULL;
	s->onMouseDown = NULL;
	s->onMouseUp = NULL;
	s->onClick = NULL;
    free(s);
}

sprite* sprite_create(char* name, int frames)
{
    sprite* s = (sprite*)malloc(sizeof(sprite));

    s->x = 0;
    s->y = 0;
    s->scaleX = 1;
    s->scaleY = 1;
    s->rotation = 0;
    s->currentFrame = 1;
	s->mouseDown = 0;
	
	s->mouseEnabled = 0;

    s->tex = assets_getTexture(name);
	SDL_QueryTexture(s->tex, NULL, NULL, &(s->width), &(s->height));

	s->width /= frames;

	s->onMouseDown = NULL;
	s->onMouseUp = NULL;
	s->onClick = NULL;

    return s;
}

void sprite_setPosition(sprite* s, int x, int y)
{
	s->x = x;
	s->y = y;
}

int sprite_handleMouse(sprite* s, SDL_Event* e)
{
	if (s->mouseEnabled == 0) return 0;
	// mouse
	SDL_Rect m;
	SDL_GetMouseState(&m.x, &m.y);
	
	m.w = 1;
	m.h = 1;

	SDL_Rect aabb;
	aabb.x = s->x - s->width / 2;
	aabb.y = s->y - s->height / 2; 
	aabb.w = s->width;
	aabb.h = s->height;


	int inside = SDL_HasIntersection(&m, &aabb);

	switch (e->type)
	{
		case SDL_MOUSEBUTTONDOWN:
			if (inside)
			{
				// handle onMouseDown
				if (s->onMouseDown != NULL) s->onMouseDown(s);
				s->mouseDown = 1;
				
				return 1;
			}	
			break;
		
		case SDL_MOUSEBUTTONUP:
			if (inside && s->mouseDown)
			{
				// click
				if (s->onClick != NULL) s->onClick(s);
			}
			if (s->mouseDown)
			{
				// reset
				if (s->onMouseUp != NULL) s->onMouseUp(s);
				s->mouseDown = 0;
			}
			break;
	}

	return 0;	
}

void sprite_draw(sprite* s)
{
        SDL_Rect dst;
        dst.w = s->width * s->scaleX;
        dst.h = s->height * s->scaleY;
		dst.x = s->x - dst.w / 2;
        dst.y = s->y - dst.h / 2;
        
        SDL_Rect src;
        src.x = s->width * (s->currentFrame - 1);
        src.y = 0;
        src.w = s->width;
        src.h = s->height;
   
        SDL_RenderCopyEx(ren, s->tex, &src, &dst, s->rotation, NULL, 0);
}

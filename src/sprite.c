#include "sprite.h"
#include "assets.h"

void sprite_free(sprite* s)
{
    s->tex = NULL;
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
    s->tex = assets_getTexture(name);
	SDL_QueryTexture(s->tex, NULL, NULL, &(s->width), &(s->height));

	s->width /= frames;

    return s;
}

void sprite_handleMouse(sprite* s, SDL_Event* e)
{
	switch (e->type)
	{
		case 1:
			break;
	}	
}

void sprite_draw(sprite* s, SDL_Renderer* ren)
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

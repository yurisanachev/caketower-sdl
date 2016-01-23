#include "sprite.h"

void sprite_free(sprite* s)
{
    s->tex = NULL;
    free(s);
}

sprite* sprite_create()
{
    sprite* s = (sprite*)malloc(sizeof(sprite));

    s->x = 0;
    s->y = 0;
    s->scaleX = 1;
    s->scaleY = 1;
    s->rotation = 0;
    s->currentFrame = 1;
    //s->tex = assets_getTexture();

    return s;
}

void sprite_draw(sprite* s, SDL_Renderer* ren)
{
        SDL_Rect dst;
        dst.x = s->x;
        dst.y = s->y;
        dst.w = s->width * s->scaleX;
        dst.h = s->height * s->scaleY;

        SDL_Rect src;
        src.x = s->width * (s->currentFrame - 1);
        src.y = 0;
        src.w = s->width;
        src.h = s->height;
   
        SDL_RenderCopyEx(ren, s->tex, &src, &dst, s->rotation, NULL, 0);
}

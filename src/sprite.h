#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <SDL.h>

typedef struct {
    int x;
    int y;
	int width;
	int height;
    double scaleX;
    double scaleY;
    double rotation;
    int currentFrame;
    SDL_Texture* tex;
} sprite;

void sprite_free(sprite* s);
sprite* sprite_create();
void sprite_draw(sprite* s, SDL_Renderer* ren);

#endif

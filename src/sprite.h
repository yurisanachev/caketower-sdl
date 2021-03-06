#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <SDL.h>
#include "engine.h"

typedef struct {
	int t;
	int x;
	int y;
	int width;
	int height;
	double scaleX;
	double scaleY;
	double rotation;
	int currentFrame;
	int mouseDown;
	int mouseEnabled;
	int visible;
	SDL_Texture* tex;

	// event handlers
	void (*onMouseDown)(entity*);
	void (*onMouseUp)(entity*);
	void (*onClick)(entity*);
} sprite;

void sprite_free(sprite* s);
sprite* sprite_create();
void sprite_draw(sprite* s);
int sprite_handleMouse(sprite* s, SDL_Event* e);
void sprite_setPosition(sprite* s, int x, int y);


#endif

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <SDL.h>
#include "sprite.h"
#include "list.h"

#define SPRITE 1
#define TEXTFIELD 2

extern SDL_Window* window;
extern SDL_Renderer* ren;
extern list_t* entities;

typedef struct {
	int t;
	int x;
	int y;
} entity;

int engine_init(char* name, int width, int height);
int engine_handleEvent(SDL_Event* e);
void engine_draw();
void engine_update();
int engine_updateFrame(int prevTime);
void engine_destroy();

void engine_addEntity(entity*);
void engine_removeEntity(entity*);
void engine_setEntityPosition(entity*, int, int);
#endif

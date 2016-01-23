#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <SDL.h>
#include "sprite.h"
#include "list.h"

extern SDL_Window* window;
extern SDL_Renderer* ren;
extern list_t* entities;

int engine_init(char* name, int width, int height);
int engine_handleEvent(SDL_Event* e);
void engine_draw();
void engine_update();
int engine_updateFrame(int prevTime);
void engine_destroy();

void engine_addEntity(void*);
void engine_removeEntity(void*);

#endif

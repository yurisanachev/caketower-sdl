#ifndef _TEXTFIELD_H_
#define _TEXTFIELD_H_

#include "list.h"
#include <SDL.h>
#include "engine.h"

typedef struct {
	int t;
	int x;
	int y;		
	int width;
	int height;
	SDL_Texture* tex;
	char* text;
	char* chars;
} textfield;

void textfield_free(textfield* t);
void textfield_setText(textfield* t, char* text);
textfield* textfield_create(char* text, char* font, char* chars);
void textfield_draw(textfield* t);
void textfield_setPosition(textfield* t, int x, int y);


#endif

#include "textfield.h"
#include <string.h>
#include "engine.h"
#include "assets.h"

void textfield_free(textfield* t)
{
    t->tex = NULL;
    free(t);
}

textfield* textfield_create(char* text, char* font, char* chars)
{
	textfield* t = (textfield*)malloc(sizeof(textfield));

    t->t = TEXTFIELD;

    t->x = 0;
    t->y = 0;

    t->tex = assets_getTexture(font);

	SDL_QueryTexture(t->tex, NULL, NULL, &(t->width), &(t->height));

	t->width /= strlen(chars);
	t->text = text;
	t->chars = chars;	

    return t;
}

void textfield_setText(textfield* t, char* text)
{
	t->text = text;
}

void textfield_setPosition(textfield* t, int x, int y)
{
	t->x = x;
	t->y = y;
}

void textfield_draw(textfield* t)
{
	size_t pos = 0;
	int charIndex;        


	while (*(t->text + pos) != '\0')
	{
		charIndex = (int)(strchr(t->chars, *(t->text + pos)) - t->chars);				
		if (charIndex > -1)
		{
			SDL_Rect dst;
	        dst.w = t->width;
	        dst.h = t->height;
			dst.x = t->x - dst.w / 2 + pos * t->width * 0.8;
	        dst.y = t->y - dst.h / 2;
	        
	        SDL_Rect src;
	        src.x = t->width * charIndex;
	        src.y = 0;
	        src.w = t->width;
	        src.h = t->height;
	   
	        SDL_RenderCopyEx(ren, t->tex, &src, &dst, 0, NULL, 0);
		}
		pos++;
	}
}

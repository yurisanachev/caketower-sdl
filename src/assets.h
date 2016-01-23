#ifndef _ASSETS_H_
#define _ASSETS_H_

#include <SDL.h>
#include <SDL_image.h>
#include <string.h>

typedef struct {
	void* src;
	char* name;
	enum {TEXTURE, SFX} type;
} asset;

void assets_free();

extern asset* assets;
extern int assetsCount;
//SDL_Sound* sfx;

SDL_Texture* assets_getTexture(char* name);

void assets_loadTexture(char* name, char* path, SDL_Renderer* ren);
//void assets_loadSfx();
#endif

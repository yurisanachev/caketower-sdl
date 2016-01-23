#include "assets.h"

asset* assets = NULL;
int assetsCount = 0;

void assets_free()
{

	// clear all assets mem here
	for (int i = 0; i < assetsCount; i++)
	{
		if (assets[i].type == TEXTURE)
		{
			SDL_DestroyTexture((SDL_Texture*)(assets[i].src));
			assets[i].src = NULL;
		}
	}

	assetsCount = 0;
	free(assets);
}

SDL_Texture* assets_getTexture(char* name)
{
	// basically loop through array and return texture
	for (int i = 0; i < assetsCount; i++)
	{
		if (assets[i].type == TEXTURE && strcmp(assets[i].name, name) == 0)
		{
			return (SDL_Texture*)(assets[i].src);
		}
	}
	
	return NULL;
}

void assets_loadTexture(char* name, char* path, SDL_Renderer* renderer)
{
	// realloc mem to load more data
	assetsCount++;
	assets = (asset*)realloc(assets, assetsCount * sizeof(asset));

	// configure new asset	
	assets[assetsCount - 1].name = name;
	assets[assetsCount - 1].src = IMG_LoadTexture(renderer, path);
	assets[assetsCount - 1].type = TEXTURE;
}

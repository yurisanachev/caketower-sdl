#include "engine.h"
#include "sprite.h"
#include "assets.h"
#include "game.h"
#include "tween.h"

#define FPS 60

SDL_Window* window = NULL;
SDL_Renderer* ren = NULL;

list_t* entities = NULL;

void engine_addEntity(void* data)
{
	list_add_back(&entities, data);
}

void engine_removeEntity(void* data)
{
	list_remove(&entities, data);
}

int engine_init(char* name, int width, int height)
{
	// init SDL2
	
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
   
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        puts("Epic fail!");
		return 1;
    }
    
	window = SDL_CreateWindow(name,
                            100,
                            100,
                            width,
                            height,
                            SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
        puts("Epic fail!");
		return 1;
	}

	
    ren = SDL_CreateRenderer(window,
								 -1,
					 SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (ren == NULL)
	{
        puts("Epic fail!");
		return 1;
	}

	// load assets
	assets_loadTexture("bg1", "../assets/bg1.png", ren);
	assets_loadTexture("bg2", "../assets/bg2.png", ren);
	assets_loadTexture("pause", "../assets/pause_button.png", ren);

	// setup game
	game_init();

	return 0;
}

int engine_handleEvent(SDL_Event* e)
{
	if (e->type == SDL_QUIT)
	{   
        return 0;
    }

	// handle mouse	
	if ((e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) &&
		e->button.button == SDL_BUTTON_LEFT)
	{
		list_t* it = entities;
		while (it)
		{
			sprite_handleMouse((sprite*)(it->value), e);
		
			it = it->next;
		}
	}
	
	return game_handleEvent(e);
}

void engine_update()
{
	// do smth here

	// update tweens
	list_t* it = tweens;
	list_t* safe;

	while (it)
	{
		safe = it->next;
		tween_update((tween*)(it->value));

		it = safe;
	}
	
	printf("Live tweens: %d \n", list_length(tweens));
}

void engine_draw()
{
	SDL_RenderClear(ren);
	
	// draw stuff
	list_t* it = entities;

	while (it)
	{
		sprite_draw((sprite*)(it->value));

		it = it->next;
	}

	SDL_RenderPresent(ren);
}

int engine_updateFrame(int prevTime)
{
	int delayTime = 1000 / FPS - (SDL_GetTicks() - prevTime);
   	if (delayTime > 0)
      	 SDL_Delay(delayTime);
	
    return SDL_GetTicks();
}

void engine_destroy()
{
	// remove all entities as well
	list_free(&entities);
	
	// remove all tweens
	list_t* it = tweens;
	
	while (it)
	{
		tween_destroy((tween*)(it->value));
		it = it->next;
	}

	list_free(&tweens);
	
	
	assets_free();
   
	game_free();
 
 	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow( window );
	SDL_Quit();
}

#include "engine.h"
#include "sprite.h"
#include "textfield.h"
#include "assets.h"
#include "game.h"
#include "tween.h"
#include <time.h>

#define FPS 60

SDL_Window* window = NULL;
SDL_Renderer* ren = NULL;

list_t* entities = NULL;

void engine_addEntity(entity* data)
{
	list_add_back(&entities, (void*)data);
}

void engine_removeEntity(entity* data)
{
	list_remove(&entities, (void*)data);
	tween_killTweensOf(data);
}

void engine_setEntityPosition(entity* data, int x, int y)
{
	data->x = x;
	data->y = y;
}

int engine_init(char* name, int width, int height)
{
	srand(time(NULL));

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
	assets_loadTexture("baker", "../assets/baker.png", ren);
	assets_loadTexture("plate", "../assets/plate.png", ren);
	assets_loadTexture("table", "../assets/table.png", ren);
	assets_loadTexture("logo", "../assets/logo.png", ren);
	assets_loadTexture("arm", "../assets/arm.png", ren);
	assets_loadTexture("gameover", "../assets/gameover.png", ren);
	assets_loadTexture("timeup", "../assets/timeup.png", ren);
	assets_loadTexture("cake", "../assets/cake.png", ren);
	assets_loadTexture("font", "../assets/font.png", ren);
	assets_loadTexture("clock", "../assets/clock.png", ren);
	assets_loadTexture("clouds", "../assets/clouds.png", ren);
	assets_loadTexture("tutorial", "../assets/tutorial.png", ren);
	assets_loadTexture("about_screen", "../assets/about_screen.png", ren);
	
	assets_loadTexture("about", "../assets/about_button.png", ren);
	assets_loadTexture("sound", "../assets/sound_button.png", ren);
	assets_loadTexture("play", "../assets/play_button.png", ren);

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
			if (((entity*)(it->value))->t == SPRITE) 
			{
				if (sprite_handleMouse((sprite*)(it->value), e)) return 1;
			}
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

	game_update();
}

void engine_draw()
{
	SDL_RenderClear(ren);
	
	// draw stuff
	list_t* it = entities;
	entity* e;

	while (it)
	{
		e = (entity*)(it->value);
		if (e->t == SPRITE) sprite_draw((sprite*)e);
		if (e->t == TEXTFIELD) textfield_draw((textfield*)e);

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
	game_free();
	
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
   
 
 	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow( window );
	SDL_Quit();
}

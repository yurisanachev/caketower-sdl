#include "engine.h"
#include "sprite.h"
#include "assets.h"

#define FPS 60

SDL_Window* window = NULL;
SDL_Renderer* ren = NULL;

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

	return 0;
}

int engine_handleEvent(SDL_Event* e)
{
	if (e->type == SDL_QUIT)
	{   
        return 0;
    } else if (e->type == SDL_MOUSEBUTTONDOWN)
   	{   
        if (e->button.button == SDL_BUTTON_LEFT)
       	{
                
        }
    }

	return 1;
}

void engine_update()
{
	// do smth here
}

void engine_draw()
{
	SDL_RenderClear(ren);
	
	// draw stuff

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

	assets_free();
    
 	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow( window );
	SDL_Quit();
}

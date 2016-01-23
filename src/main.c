#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "assets.h"

#define FPS 60

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int rot = 0;
int targetTime = 1000 / FPS;
int frameStartTime = 0;
int delayTime = 0;

int main( int argc, char* args[] )
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;
	
	SDL_Renderer* ren = NULL;	
	
	// smoothing enabled
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return 1;
	} 
	//Create window
	window = SDL_CreateWindow("Cake Tower", 
							100, 
							100, 
							SCREEN_WIDTH, 
							SCREEN_HEIGHT, 
							SDL_WINDOW_SHOWN);
	ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


	assets_loadTexture("baker", "../assets/baker.png", ren);

	SDL_Event e;
	
	int running = 1;
	
	while (running)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				running = 0;
			} else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{

				}
			}	
		}	
		
		rot++;
		if (rot > 360) rot -= 360;

	
		SDL_RenderClear(ren);

		SDL_RenderPresent(ren);
	
		delayTime = targetTime - (SDL_GetTicks() - frameStartTime);
		if (delayTime > 0)
			SDL_Delay(delayTime);

		frameStartTime = SDL_GetTicks();
	}
	
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}

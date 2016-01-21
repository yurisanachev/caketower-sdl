#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;




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
	ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


	SDL_Texture *tex = IMG_LoadTexture(ren, "../assets/baker.png");	

	/*for (int i = 0; i < 10; i++)
	{
			SDL_Delay(500);
	}*/	
		
	SDL_Event e;
	
	int running = 1;
	
	while (running)
	{
		SDL_WaitEvent(&e);
		
		switch (e.type)
		{
			case SDL_QUIT:
				running = 0;
				break;
			
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT)
				{

				}
	
				break;
		}	

		SDL_RenderClear(ren);
		SDL_RenderCopy(ren, tex, NULL, NULL);
		SDL_RenderPresent(ren);
	
	}
	
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}

#include <SDL.h>
#include <stdio.h>
#include "engine.h"

#define WIDTH 640
#define HEIGHT 720

int main( int argc, char* args[] )
{

	// init engine here
	if (engine_init("Cake Tower", WIDTH, HEIGHT) != 0)
	{
		return 1;
	}

	SDL_Event e;
	int running = 1;
	int prevTime = 0;	
	
	while (running)
	{
		// handle events
		while (SDL_PollEvent(&e))
			running = engine_handleEvent(&e);

		// update game
		engine_update();
	
		// draw shit
		engine_draw();
		
		// update FPS	
		prevTime = engine_updateFrame(prevTime);
	}

	// the end
	engine_destroy();	
	
	return 0;
}

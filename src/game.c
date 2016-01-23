#include "game.h"
#include "callbacks.h"
#include "engine.h"	
#include "tween.h"

game_buttons* b = NULL;
game_environment* env = NULL;


void game_init()
{
	// create containers
	b = (game_buttons*)malloc(sizeof(game_buttons));
	env = (game_environment*)malloc(sizeof(game_environment));
	
	env->bg1 = sprite_create("bg1", 1);
	sprite_setPosition(env->bg1, 320, 360);

	env->bg2 = sprite_create("bg2", 1);
	sprite_setPosition(env->bg2, 320, 426);
	
	// MENU
	
	// play button
	b->play = sprite_create("pause", 2);
	sprite_setPosition(b->play, 320, 360);

	engine_addEntity((void*)(env->bg1));
	engine_addEntity((void*)(env->bg2));
	engine_addEntity((void*)(b->play));	


	b->play->onMouseDown = &callback_buttonDownDefault;	
	b->play->onMouseUp = &callback_buttonUpDefault;	
	b->play->onClick = &callback_startGameFromMenu;	
}

int game_handleEvent(SDL_Event* e)
{
	if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT)  
	{
		// handle click
	}

	return 1;	
}

void game_draw()
{
}

void game_free()
{
	// destroy buttons
	sprite_free(b->play);
	


	free(b);

	// destroy environment
	sprite_free(env->bg1);
	sprite_free(env->bg2);



	free(env);
}
// do smth here

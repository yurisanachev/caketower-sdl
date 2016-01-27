#include "game.h"
#include "callbacks.h"
#include "engine.h"	
#include "tween.h"

game_buttons* b = NULL;
game_environment* env = NULL;
game* g = NULL;

void game_reset()
{
	g->playing = 0;
	g->gameOver = 0;
	g->dropping = 0;
	g->score = 0;
	g->speed = 5;
	g->dir = 1;
	g->lastCakeY = 550;
	g->cakesAvailable = 5;
	g->cakeAmount = 6;
	g->cakeSize = 80;
	g->dropSpeed = 0;
	g->timeLeft = 100;


	// clear all cakes to drop
	list_t* it = g->cakesToDrop;
	
	while (it)
	{	
		engine_removeEntity(it->value);
		sprite_free((sprite*)(it->value));
		it = it->next;
	}

	list_free(&(g->cakesToDrop));	


	// clear all other cakes
	it = g->cakes;
	
	while (it)
	{	
		engine_removeEntity(it->value);
		sprite_free((sprite*)(it->value));
		it = it->next;
	}

	list_free(&(g->cakes));
	
}

void game_init()
{
	// create containers
	b = (game_buttons*)malloc(sizeof(game_buttons));
	g = (game*)malloc(sizeof(game));

	g->cakesToDrop = NULL;
	g->cakes = NULL;

	env = (game_environment*)malloc(sizeof(game_environment));

	// bg1	
	env->bg1 = sprite_create("bg1", 1);
	sprite_setPosition(env->bg1, 320, 360);

	// bg2
	env->bg2 = sprite_create("bg2", 1);
	sprite_setPosition(env->bg2, 320, 426);

	// baker	
	env->baker = sprite_create("baker", 1);
	
	// plate
	env->plate = sprite_create("plate", 1);


	// logo
	env->logo = sprite_create("logo", 1);
	
	// table
	env->table = sprite_create("table", 1);

	// timeup
	env->timeup = sprite_create("timeup", 1);
	env->timeup->scaleX = 0;
	env->timeup->scaleY = 0;

	// arm
	env->arm = sprite_create("arm", 1);
	sprite_setPosition(env->arm, 0, -180);


	// aboutScreen
	env->aboutScreen = sprite_create("about_screen", 1);
	sprite_setPosition(env->aboutScreen, 320, -220);	

	// game over	
	env->gameover = sprite_create("gameover", 1);	
	sprite_setPosition(env->gameover, 320, -220);	

	// MENU
	
	// play button
	b->play = sprite_create("play", 2);
	b->play->mouseEnabled = 1;
	
	engine_addEntity((void*)(env->bg1));
	engine_addEntity((void*)(env->bg2));
	engine_addEntity((void*)(env->baker));
	engine_addEntity((void*)(env->logo));
	engine_addEntity((void*)(env->table));
	engine_addEntity((void*)(env->plate));
	engine_addEntity((void*)(env->timeup));
	engine_addEntity((void*)(env->gameover));
	engine_addEntity((void*)(env->arm));
	engine_addEntity((void*)(env->aboutScreen));

	engine_addEntity((void*)(b->play));	
	

	b->play->onMouseDown = &callback_buttonDownDefault;	
	b->play->onMouseUp = &callback_buttonUpDefault;	
	b->play->onClick = &callback_startGameFromMenu;	

	game_showMenu();
}

int game_handleEvent(SDL_Event* e)
{
	if (g->playing && g->gameOver == 0 && g->dropping == 0)
	{
		if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)  
		{
			// handle click somehow
			//game_finishGame();
			g->dropping = 1;
			//g->lastCakeY -= g->cakeSize;
			g->dropSpeed = 7;
		}
	}


	return 1;	
}

void game_showGame()
{
	// reset game
	game_reset();

	// reset plate
	sprite_setPosition(env->plate, 940, 635);

	// TODO: on Complete launch gameplay!!!!
	tween_create(env->plate, 320, env->plate->y, 1, 1, 0, 500, 500, NULL, &game_startGame);


	// config table
			
}

void game_startGame()
{
	g->playing = 1;

	game_makeNewCake();
}

void game_update()
{
	
	if (g->playing)
	{
		// update time left
		if (g->timeLeft > -1)
		{
			g->timeLeft--;
			
			if (g->timeLeft == -1)
			{
				g->gameOver = 1;
				if (g->dropping == 0)
				{
					game_finishGame();
				}
			}
		}
		

		if (g->dropping == 0 && g->gameOver == 0)
		{
			// move stuff around
			if (env->arm->x > 625 || env->arm->x < -15)
            {
                g->dir *= -1;
            }
 
            env->arm->x += g->dir * g->speed;
			
			list_t* it = g->cakesToDrop;

			while (it)
			{
				((sprite*)(it->value))->x += g->dir * g->speed;
				it = it->next;
			}			

		} else if (g->dropping) 
		{ 
			// do here smth !!!!
			env->arm->y -= 10;

			list_t* it = g->cakesToDrop;
	
			while (it)
			{
				sprite* c = (sprite*)(it->value);	
			
				if (c->y == g->lastCakeY)
				{
					g->dropping = 0;
				
					game_makeNewCake();
					break;
				}

				if (g->lastCakeY - c->y < g->dropSpeed)
				{
					c->y = g->lastCakeY;
				} else {
					c->y += g->dropSpeed;
				}
			
				c->scaleX -= 0.005;
				c->scaleY -= 0.01;	
				

	
				it = it->next;
			}

			g->dropSpeed++;	
		}	
	}		


		


}

void game_makeNewCake()
{
	env->arm->y = 80;	
	
	game_makeFallCheck();

	if (list_length(g->cakesToDrop) > 0)
	{
		// attach dropped one
		if (g->speed < 25) g->speed += 0.5; 

		list_t* it = g->cakes;

		while (it)
		{
			sprite* c = (sprite*)(it->value);
			
			list_t* safe = it->next;

			if (c->y > 720)
			{
				// just remove those cakes
				list_remove(&(g->cakes), (void*)c);
				engine_removeEntity(c);	
				sprite_free(c);
	
			} else {
				tween_create(c, c->x, c->y + g->cakeSize, 1, 1, 0, 500, 0, &backOut, NULL);
			}

			it = safe;
		}
		
		it = g->cakesToDrop;
		
		while (it)
		{
			sprite* c = (sprite*)(it->value);
			tween_create(c, c->x, c->y + g->cakeSize, 1, 1, 0, 500, 0, &backOut, NULL);
			
			list_add_back(&(g->cakes), it->value);	
			it = it->next;
		}	
		
		tween_create(env->plate, 320, env->plate->y + g->cakeSize, 1, 1, 0, 500, 0, &backOut, NULL);
		tween_create(env->table, 320, env->table->y + g->cakeSize, 1, 1, 0, 500, 0, &backOut, NULL);
		tween_create(env->bg2, 320, env->bg2->y + g->cakeSize / 8, 1, 1, 0, 300, 0, NULL, NULL);
	}
	
	list_free(&(g->cakesToDrop));

	env->arm->x = rand() % 10 > 5 ? -15 : 625;

	int startX = 0;

	for (int i = 0; i < g->cakeAmount; i++)
	{
		sprite* c = sprite_create("cake", 11);
	
		if (g->cakeAmount % 2 != 0) startX = env->arm->x - (g->cakeAmount - 1) / 2 * g->cakeSize;
		else startX = env->arm->x - g->cakeSize / 2 - (g->cakeAmount / 2 - 1) * g->cakeSize;
		
		
		startX += 30;

		sprite_setPosition(c, startX + i * g->cakeSize, env->arm->y + 90);

		list_add_back(&(g->cakesToDrop), c);
		engine_addEntity(c);
	}

	if (g->gameOver) game_finishGame();
}

void game_makeFallCheck()
{
	int len = list_length(g->cakes);
	
	//int leftCheck = len > 0 ? 	
	
}

void game_finishGame()
{
	if (g->playing == 0) return;
	g->playing = 0;
	
	// show here shit like time up
	env->timeup->scaleX = 0;
	env->timeup->scaleY = 0;
	
	sprite_setPosition(env->timeup, 320, 360);
	env->timeup->rotation = 50;
	
	tween_create(env->timeup, 320, 360, 1.2, 1.2, 0, 2000,   0, &elasticOut, NULL);

	env->timeup->scaleX = 1.2;
	env->timeup->scaleY = 1.2;	
	env->timeup->rotation = 0;	
	
	tween_create(env->timeup, 1000, 360, 1.2, 1.2, 0, 500, 2000,  &backIn, NULL);

	env->timeup->scaleX = 0;
	env->timeup->scaleY = 0;	
	env->timeup->rotation = 50;	
	

	
	// hide table	
	tween_create(env->table, 320, env->table->y + 360, 1, 1, 0, 500, 2300, NULL, NULL);		
	
	// hide plate as well
	tween_create(env->plate, 320, env->plate->y + 360, 1, 1, 0, 500, 2300, NULL, NULL);		

	// hide arm
	tween_create(env->arm, env->arm->x, -180, 1, 1, 0, 300, 0, NULL, NULL);

	// hide available cakes
	list_t* it = g->cakesToDrop;
	sprite* curr;	

	while (it)
	{	
		curr = (sprite*)(it->value);
		tween_create(curr, curr->x, -200, 1, 1, 0, 300, 0, NULL, NULL);	
	
		it = it->next;
	}	


	// hide tower as well
	it = g->cakes;	

	while (it)
	{	
		curr = (sprite*)(it->value);
		tween_create(curr, curr->x, curr->y + 220, 1, 1, 0, 300, 2300, NULL, NULL);	
	
		it = it->next;
	}

	// tween bg
	
	tween_create(env->bg2, 320, env->bg2->y + 160, 1, 1, 0, 500, 2400, &expoOut, NULL);		

	// tween gameover
	tween_create(env->gameover, 320, 300, 1, 1, 0, 1000, 2800, &elasticOut, &game_hideGame);
}

void game_hideGame()
{
	// hide level stuff and/or gameover/pause
	
	// hide gameover	
	tween_create(env->gameover, 320, -220, 1, 1, 0, 500, 0, &backIn, NULL);
	

	
	// reset stats
	game_reset();

	// show menu
	game_showMenu();
}

void game_hideMenu()
{
	tween_create(env->bg2, env->bg2->x, env->bg2->y, 1, 1, 0, 500, 0, NULL, NULL);


	tween_create(env->baker, -200, env->baker->y, 1, 1, 0.7 * 180 / M_PI, 500, 0, NULL, NULL);	

	
	tween_create(env->logo, env->logo->x, -200, 1, 1, 0, 600, 0, &backIn, NULL);

	tween_create(b->play, b->play->x, 870, 1, 1, 0, 500, 0, &backIn, NULL);
}

void game_showMenu()
{
	// setup bg2

	tween_create(env->bg2, 320, 426, 1.2, 1.2, 0, 500, 0, NULL, NULL);


	// setup baker
	sprite_setPosition(env->baker, 315, 1030);
	env->baker->rotation = 0.7 * 180 / M_PI;
	
	tween_create(env->baker, env->baker->x, 466, 1, 1, 0, 500, 0, NULL, NULL);	

	
	// setup logo
	sprite_setPosition(env->logo, 190, 150);
	env->logo->scaleX = env->logo->scaleY = 0;
	
	tween_create(env->logo, env->logo->x, env->logo->y, 1, 1, 0, 600, 400, &elasticOut, NULL);

	// table
	sprite_setPosition(env->table, 320, 800);
	tween_create(env->table, env->table->x, 695, 1, 1, 0, 500, 0, NULL, NULL);	


	// play button
	sprite_setPosition(b->play, 425, 870);

	tween_create(b->play, b->play->x, 395, 1, 1, 0, 500, 600, &backOut, NULL);

	// plate	
	sprite_setPosition(env->plate, 940, 635);

}



void game_free()
{
	game_reset();

	// destroy buttons
	
	engine_removeEntity((void*)(b->play));
	sprite_free(b->play);
	


	free(b);

	// destroy environment
	

	engine_removeEntity((void*)(env->bg1));
	sprite_free(env->bg1);
	
	engine_removeEntity((void*)(env->bg2));
	sprite_free(env->bg2);
	
	engine_removeEntity((void*)(env->baker));
	sprite_free(env->baker);
	
	engine_removeEntity((void*)(env->plate));
	sprite_free(env->plate);
	
	engine_removeEntity((void*)(env->logo));
	sprite_free(env->logo);
	
	engine_removeEntity((void*)(env->table));
	sprite_free(env->table);
	
	engine_removeEntity((void*)(env->timeup));
	sprite_free(env->timeup);
	
	engine_removeEntity((void*)(env->gameover));
	sprite_free(env->gameover);
	
	engine_removeEntity((void*)(env->aboutScreen));
	sprite_free(env->aboutScreen);
	
	engine_removeEntity((void*)(env->arm));
	sprite_free(env->arm);


	free(env);

	free(g);
}
// do smth here

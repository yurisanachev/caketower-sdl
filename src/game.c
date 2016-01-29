#include "game.h"
#include "callbacks.h"
#include "engine.h"	
#include "tween.h"
#include <math.h>

#define CAKE_MAX_Y 550
#define DEADLINE_TIME 10

game_buttons* b = NULL;
game_environment* env = NULL;
game* g = NULL;

void game_reset()
{
	g->playing = 0;
	g->gameOver = 0;
	g->dropping = 0;
	g->score = 0;
	g->speed = 7;
	g->dir = 1;
	g->dt = 0;
	g->cakesAvailable = 5;
	g->cakeAmount = 6;
	g->prevCakeAmount = 6;
	g->cakeSize = 80;
	g->dropSpeed = 0;
	g->timeLeft = 59;
	g->totalHeight = 0;

	// clear all cakes to drop
	game_freeSpriteList(&(g->cakesToDrop));
	
	// clear all other cakes
	game_freeSpriteList(&(g->cakes));

	// clear falling
	game_freeSpriteList(&(g->falling));
	
	// clear clouds
	game_freeSpriteList(&(g->clouds));
	
	// clear shouts
	game_freeSpriteList(&(g->shouts));
}

void game_freeSpriteList(list_t** head)
{
	list_t* it = *head;
	
	while (it)
	{	
		engine_removeEntity((entity*)(it->value));
		sprite_free((sprite*)(it->value));
		it = it->next;
	}

	list_free(head);
	
}

void game_init()
{
	// create containers
	b = (game_buttons*)malloc(sizeof(game_buttons));
	g = (game*)malloc(sizeof(game));

	g->cakesToDrop = NULL;
	g->cakes = NULL;
	g->falling = NULL;
	g->clouds = NULL;
	g->shouts = NULL;

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

	
	// clock
	env->clock = sprite_create("clock", 1);
	sprite_setPosition(env->clock, 40, 800);

	// tutorial
	env->tutorial = sprite_create("tutorial", 1);
	sprite_setPosition(env->tutorial, 850, 420);	
	
	// game over	
	env->gameover = sprite_create("gameover", 1);	
	sprite_setPosition(env->gameover, 320, -220);	

	// time remaining field
	env->timeField = textfield_create("05:00", "font", "0123456789:");
	textfield_setPosition(env->timeField, 75, 800);

	// score field	
	env->scoreField = textfield_create("0000000", "font", "0123456789:");
	textfield_setPosition(env->scoreField, 450, 800);


	// MENU
	
	// play button
	b->play = sprite_create("play", 2);
	b->play->mouseEnabled = 1;
	
	engine_addEntity((entity*)(env->bg1));
	engine_addEntity((entity*)(env->bg2));
	engine_addEntity((entity*)(env->baker));
	engine_addEntity((entity*)(env->logo));
	engine_addEntity((entity*)(env->table));
	engine_addEntity((entity*)(env->plate));
	engine_addEntity((entity*)(env->timeup));
	engine_addEntity((entity*)(env->gameover));
	engine_addEntity((entity*)(env->clock));
	engine_addEntity((entity*)(env->arm));
	engine_addEntity((entity*)(env->tutorial));
	engine_addEntity((entity*)(env->timeField));
	engine_addEntity((entity*)(env->scoreField));

	engine_addEntity((entity*)(b->play));	
	

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
			// drop cake!	
			g->dropping = 1;
			g->dropSpeed = 7;
		
			if (env->tutorial->scaleX == 1)
			{
				tween_create((entity*)(env->tutorial), 320, 420, 0, 0, 0, 500, 0, &backIn, NULL);
			}
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

	// on Complete launch gameplay!!!!
	tween_create((entity*)(env->plate), 320, env->plate->y, 1, 1, 0, 500, 500, NULL, &game_startGame);

	// tutorial
	sprite_setPosition(env->tutorial, 320, 420);
	env->tutorial->scaleX = env->tutorial->scaleY = 0;	
	tween_create((entity*)(env->tutorial), 320, 420, 1, 1, 0, 500, 600, &elasticOut, NULL);

	// clock icon
	sprite_setPosition(env->clock, 40, 800);
	tween_create((entity*)(env->clock), 35, 680, 1, 1, 0, 500, 600, NULL, NULL);	
	
	// time field
	engine_setEntityPosition((entity*)(env->timeField), 85, 800);
	tween_create((entity*)(env->timeField), 85, 680, 1, 1, 0, 500, 700, NULL, NULL);


	// score field
	engine_setEntityPosition((entity*)(env->scoreField), 425, 800);
	tween_create((entity*)(env->scoreField), 425, 680, 1, 1, 0, 500, 800, NULL, NULL);


	// make them visible
	env->timeField->visible = 1;
	env->clock->visible = 1;	

	// config table
			
}

void game_startGame(entity* sender)
{
	g->playing = 1;

	game_makeNewCake();
}

void game_update()
{
		
	// update time
	char* timeText = malloc(sizeof(char) * 5);
	if (g->gameOver == 0) sprintf(timeText, "%02d:%02d", g->timeLeft, 60 - g->dt);
	else sprintf(timeText, "00:00");
	textfield_setText(env->timeField, timeText);
	free(timeText);   
 

	if (g->playing)
	{
		if (g->dt == 60)
		{
			g->dt = 0;
			
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
		}

		g->dt++;

		if (g->timeLeft < DEADLINE_TIME && g->dt % 15 == 0)
		{
			
			env->timeField->visible = !env->timeField->visible;			
			env->clock->visible = !env->clock->visible;			

		} else if (g->timeLeft > DEADLINE_TIME)
		{
			env->timeField->visible = env->clock->visible = 1;
		}

		// update score
		char* scoreText = malloc(sizeof(char) * 7);
		sprintf(scoreText, "%07d", g->score);
		textfield_setText(env->scoreField, scoreText);
		free(scoreText);   



		if (g->dropping == 0 && g->gameOver == 0)
		{
			// move stuff around
			if (env->arm->x > 625 || env->arm->x < -15)
            {
                g->dir *= -1;
            }
 
            env->arm->x += g->dir * g->speed;
			
			list_t* it = g->cakesToDrop;
			int pos = -(g->cakeAmount - 1) / 2 * g->cakeSize;
			
			if (g->cakeAmount % 2 == 0) pos = -g->cakeSize * (g->cakeAmount / 2 - 1) - g->cakeSize / 2;
			
			pos += 30;

			while (it)
			{
				((sprite*)(it->value))->x = env->arm->x + pos;
				pos += g->cakeSize;
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
			
				if (c->y == CAKE_MAX_Y)
				{
					g->dropping = 0;
				
					game_makeNewCake();
					break;
				}

				if (CAKE_MAX_Y - c->y < g->dropSpeed)
				{
					c->y = CAKE_MAX_Y;
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
	
	// update falling guys here
	list_t* it = g->falling;
	list_t* safe;	
	sprite* c;
	
	while (it)
	{
		c = (sprite*)(it->value);
		safe = it->next;

		if (c->y > 800)
		{
			// delete me!
			list_remove(&(g->falling), (void*)c);
			engine_removeEntity((entity*)c);	
			sprite_free(c);

		} else {
			c->y += g->dropSpeed;
		}			

		it = safe;
	}
}

void game_makeNewCake()
{
	env->arm->y = 80;	
	
	game_makeFallCheck();

	int needScroll = 0;

	if (list_length(g->cakesToDrop) > 0)
	{
		needScroll = 1;

		// attach dropped one
		if (g->speed < 25) g->speed += 0.5; 

		g->totalHeight++;	

			
		g->prevCakeAmount = g->cakeAmount;

		g->score += 70 * g->cakeAmount * g->speed;


		if (g->cakesAvailable < 11 && g->totalHeight % 10 == 0)
		{
			g->cakesAvailable++;
		}

	

		list_t* it = g->cakes;

		while (it)
		{
			sprite* c = (sprite*)(it->value);
			
			list_t* safe = it->next;

			if (c->y > 720)
			{
				// just remove those cakes
				list_remove(&(g->cakes), (void*)c);
				engine_removeEntity((entity*)c);	
				sprite_free(c);
	
			} else {
				tween_create((entity*)c, c->x, c->y + g->cakeSize, 1, 1, 0, 500, 0, &backOut, NULL);
			}

			it = safe;
		}
		
		it = g->cakesToDrop;
		
		while (it)
		{
			sprite* c = (sprite*)(it->value);
			tween_create((entity*)c, c->x, c->y + g->cakeSize, 1, 1, 0, 500, 0, &backOut, NULL);
			
			list_add_back(&(g->cakes), it->value);	
			it = it->next;
		}	
		
		tween_create((entity*)(env->plate), 320, env->plate->y + g->cakeSize, 1, 1, 0, 500, 0, &backOut, NULL);
		tween_create((entity*)(env->table), 320, env->table->y + g->cakeSize, 1, 1, 0, 500, 0, &backOut, NULL);
		tween_create((entity*)(env->bg2), 320, env->bg2->y + g->cakeSize / 8, 1, 1, 0, 300, 0, NULL, NULL);

		if (g->totalHeight % 7 == 0) 
		{
		 	game_addCloud();
		}

		engine_removeEntity((entity*)(env->scoreField));
		engine_removeEntity((entity*)(env->arm));
		engine_removeEntity((entity*)(env->timeField));
		engine_removeEntity((entity*)(env->clock));
		engine_addEntity((entity*)(env->scoreField));
		engine_addEntity((entity*)(env->timeField));
		engine_addEntity((entity*)(env->clock));
		engine_addEntity((entity*)(env->arm));
	}
	
	list_free(&(g->cakesToDrop));

	env->arm->x = rand() % 10 > 5 ? -15 : 625;

	int startX = 0;

	for (int i = 0; i < g->cakeAmount; i++)
	{
		sprite* c = sprite_create("cake", 11);

		c->currentFrame = rand() % g->cakesAvailable + 1;
	
		if (g->cakeAmount % 2 != 0) startX = env->arm->x - (g->cakeAmount - 1) / 2 * g->cakeSize;
		else startX = env->arm->x - g->cakeSize / 2 - (g->cakeAmount / 2 - 1) * g->cakeSize;
		
		
		startX += 30;

		sprite_setPosition(c, startX + i * g->cakeSize, env->arm->y + 90);

		list_add_back(&(g->cakesToDrop), c);
		engine_addEntity((entity*)c);
	}

	// tween clouds as well if we proceed!
	if (needScroll)
	{

		list_t* it = g->clouds;

		sprite* cloud;
	
		while (it)
		{
			cloud = (sprite*)(it->value);
			
			// put blurred on top	
			if (cloud->currentFrame > 4) 
			{
				engine_removeEntity((entity*)cloud);
				engine_addEntity((entity*)cloud);
			}
	
			tween_create((entity*)cloud, cloud->x, cloud->y + g->cakeSize / 3, 1, 1, 0, 600, 0, &backOut, NULL);
			it = it->next;
		}
	}


	if (g->gameOver) 
	{
		game_finishGame();
	}
}

void game_makeFallCheck()
{
	int len = list_length(g->cakes);
		
	list_t* it = g->cakes;	
	sprite* c;
	
	int leftCheck = 80;
	int rightCheck = 560;

	if (len > 0)
	{	
		c = (sprite*)(list_get(g->cakes, len - 1));
		rightCheck = c->x;

		c = (sprite*)(list_get(g->cakes, len - g->prevCakeAmount));
		leftCheck = c->x;
	}
	
	it = g->cakesToDrop;

	len = list_length(g->cakesToDrop);
	int fallen = 0;

	while (it)
	{
		c = (sprite*)(it->value);		
		list_t* safe = it->next;
		
		if ((leftCheck - c->x) > g->cakeSize || (c->x - rightCheck) > g->cakeSize)
		{
			fallen++;	
			
			list_remove(&(g->cakesToDrop), (void*)c);
			list_add_back(&(g->falling), (void*)c);
		}

		it = safe;
	}	
	
	if (fallen == 0 && len > 0 && abs(c->x - rightCheck) < 20)
	{
		// add shout
		game_addShout();
   	}

	if (len == fallen) 
	{
		// missed all cakes! :)	
	} else
	{
		g->cakeAmount -= fallen;
	}
}

void game_addShout()
{
	size_t len = list_length(g->shouts);
	sprite* shout = (len > 5) ? (sprite*)(list_shift(&(g->shouts))) : sprite_create("shouts", 5);
	
	if (len <= 5) engine_addEntity((entity*)shout);
		
	tween_killTweensOf((entity*)shout);

	shout->currentFrame = rand() % 4 + 1;
	
	// small chance	
	if (rand() % 10 < 3) 
	{
		shout->currentFrame = 5;	
		g->timeLeft++;
	}

	sprite_setPosition(shout, env->arm->x + rand() % 200 - 100, CAKE_MAX_Y - rand() % 100); 
    	
	shout->scaleX = shout->scaleY = 0;
	
	int angle = rand() % 10 > 5 ? -10 : 10;

	tween_create((entity*)shout, shout->x, shout->y - 50, 1, 1, angle, 1000, 0, &elasticOut, NULL);
	shout->scaleX = shout->scaleY = 1;
	shout->rotation = angle;
	shout->y -= 50;
	tween_create((entity*)shout, shout->x, shout->y - 50, 0, 0, -angle, 300, 1000, &backIn, NULL);

	shout->scaleX = shout->scaleY = 0;	
	shout->y += 50;
	shout->rotation = 0;

		
	list_add_back(&(g->shouts), (void*)shout);
}


void game_addCloud()
{
	int len = list_length(g->clouds);
	sprite* cloud = (len > 8) ? (sprite*)(list_shift(&(g->clouds))) : sprite_create("clouds", 6);

	if (len <= 8) engine_addEntity((entity*)cloud);
	
	cloud->currentFrame = rand() % 6 + 1;	
	cloud->scaleX = cloud->scaleY = rand() % 10 > 5 ? 1 : 1.3;	
	
	cloud->y = -80 - rand() % 30;

	len = list_length(g->clouds);
	
	if (len == 0 || ((sprite*)(list_get(g->clouds, len - 1)))->x < 320)
	{
		cloud->x = rand() % 100 + 490;
	} else
	{	
		cloud->x = rand() % 100 + 75;
	} 

	list_add_back(&(g->clouds), (void*)cloud);
	
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

	engine_removeEntity((entity*)(env->timeup));
	engine_addEntity((entity*)(env->timeup));
	
	tween_create((entity*)(env->timeup), 320, 360, 1.2, 1.2, 0, 2000,   0, &elasticOut, NULL);

	env->timeup->scaleX = 1.2;
	env->timeup->scaleY = 1.2;	
	env->timeup->rotation = 0;	
	
	tween_create((entity*)(env->timeup), 1000, 360, 1.2, 1.2, 0, 500, 2000,  &backIn, NULL);

	env->timeup->scaleX = 0;
	env->timeup->scaleY = 0;	
	env->timeup->rotation = 50;	
	

	
	// hide table
	tween_killTweensOf((entity*)(env->table));	
	tween_create((entity*)(env->table), 320, env->table->y + 360, 1, 1, 0, 500, 2300, NULL, NULL);		
	
	// hide plate as well
	tween_killTweensOf((entity*)(env->plate));	
	tween_create((entity*)(env->plate), 320, env->plate->y + 360, 1, 1, 0, 500, 2300, NULL, NULL);		

	// hide arm
	tween_create((entity*)(env->arm), env->arm->x, -180, 1, 1, 0, 300, 0, NULL, NULL);

	// hide time txt
	tween_create((entity*)(env->timeField), env->timeField->x, 800, 1, 1, 0, 500, 200, &backIn, NULL);
	
	// hide score txt
	tween_create((entity*)(env->scoreField), env->scoreField->x, 800, 1, 1, 0, 500, 100, &backIn, NULL);

	// clock icon
	tween_create((entity*)(env->clock), env->clock->x, 800, 1, 1, 0, 500, 300, &backIn, NULL);	

	// hide available cakes
	list_t* it = g->cakesToDrop;
	sprite* curr;	

	while (it)
	{	
		curr = (sprite*)(it->value);
		tween_create((entity*)curr, curr->x, -200, 1, 1, 0, 300, 0, NULL, NULL);	
	
		it = it->next;
	}	


	// hide tower as well
	it = g->cakes;	

	while (it)
	{	
		curr = (sprite*)(it->value);
		tween_killTweensOf((entity*)curr);	
		tween_create((entity*)curr, curr->x, curr->y + 220, 1, 1, 0, 300, 2300, NULL, NULL);	
	
		it = it->next;
	}

	// hide clouds
	it = g->clouds;
	int delay = 0;

	while (it)
	{
		curr = (sprite*)(it->value);
			
		tween_killTweensOf((entity*)curr);	
		tween_create((entity*)curr, (curr->x > 320) ? 800 : -180, curr->y, 1, 1, 0, 300, 2300 + delay, &backIn, NULL);
		delay += 50;
		it = it->next;
	}


	// tween bg
	
	tween_create((entity*)(env->bg2), 320, env->bg2->y + 160, 1, 1, 0, 500, 2400, &expoOut, NULL);		

	// score field
	engine_setEntityPosition((entity*)(env->scoreField), 225, -200);
	tween_create((entity*)(env->scoreField), env->scoreField->x, 370, 1, 1, 0, 1000, 2700, &elasticOut, NULL);
	
	// tween gameover
	tween_create((entity*)(env->gameover), 320, 350, 1, 1, 0, 1000, 2800, &elasticOut, NULL);

	// delay
	tween_create((entity*)(env->bg1), env->bg1->x, env->bg1->y, 1, 1, 0, 1000, 5000, NULL, &game_hideGame);
}

void game_hideGame(entity* sender)
{
	// hide level stuff and/or gameover/pause
	
	// hide gameover	
	tween_create((entity*)(env->gameover), 320, -220, 1, 1, 0, 500, 0, &backIn, NULL);
	

	// score field
	tween_create((entity*)(env->scoreField), env->scoreField->x, -200, 1, 1, 0, 500, 100, &backIn, NULL);
	
	// reset stats
	game_reset();

	// show menu
	game_showMenu();
}

void game_hideMenu()
{
	tween_create((entity*)(env->bg2), env->bg2->x, env->bg2->y, 1, 1, 0, 500, 0, NULL, NULL);


	tween_create((entity*)(env->baker), -200, env->baker->y, 1, 1, 0.7 * 180 / M_PI, 500, 0, NULL, NULL);	

	
	tween_create((entity*)(env->logo), env->logo->x, -200, 1, 1, 0, 600, 0, &backIn, NULL);

	tween_create((entity*)(b->play), b->play->x, 870, 1, 1, 0, 500, 0, &backIn, NULL);
}

void game_showMenu()
{
	// setup bg2

	tween_create((entity*)(env->bg2), 320, 426, 1.2, 1.2, 0, 500, 0, NULL, NULL);


	// setup baker
	sprite_setPosition(env->baker, 315, 1030);
	env->baker->rotation = 0.7 * 180 / M_PI;
	
	tween_create((entity*)(env->baker), env->baker->x, 466, 1, 1, 0, 500, 0, NULL, NULL);	

	
	// setup logo
	sprite_setPosition(env->logo, 190, 150);
	env->logo->scaleX = env->logo->scaleY = 0;
	
	tween_create((entity*)(env->logo), env->logo->x, env->logo->y, 1, 1, 0, 600, 400, &elasticOut, NULL);

	// table
	sprite_setPosition(env->table, 320, 800);
	tween_create((entity*)(env->table), env->table->x, 695, 1, 1, 0, 500, 0, NULL, NULL);	


	// play button
	sprite_setPosition(b->play, 425, 870);

	tween_create((entity*)(b->play), b->play->x, 395, 1, 1, 0, 500, 600, &backOut, NULL);

	// plate	
	sprite_setPosition(env->plate, 940, 635);

}



void game_free()
{
	game_reset();

	// destroy buttons
	
	engine_removeEntity((entity*)(b->play));
	sprite_free(b->play);
	


	free(b);

	// destroy environment
	

	engine_removeEntity((entity*)(env->bg1));
	sprite_free(env->bg1);
	
	engine_removeEntity((entity*)(env->bg2));
	sprite_free(env->bg2);
	
	engine_removeEntity((entity*)(env->baker));
	sprite_free(env->baker);
	
	engine_removeEntity((entity*)(env->plate));
	sprite_free(env->plate);
	
	engine_removeEntity((entity*)(env->logo));
	sprite_free(env->logo);
	
	engine_removeEntity((entity*)(env->table));
	sprite_free(env->table);
	
	engine_removeEntity((entity*)(env->timeup));
	sprite_free(env->timeup);
	
	engine_removeEntity((entity*)(env->gameover));
	sprite_free(env->gameover);
	
	engine_removeEntity((entity*)(env->tutorial));
	sprite_free(env->tutorial);
	
	engine_removeEntity((entity*)(env->arm));
	sprite_free(env->arm);

	engine_removeEntity((entity*)(env->clock));
	sprite_free(env->clock);
	
	engine_removeEntity((entity*)(env->timeField));
	textfield_free(env->timeField);
	
	engine_removeEntity((entity*)(env->scoreField));
	textfield_free(env->scoreField);
	
	free(env);

	free(g);
}

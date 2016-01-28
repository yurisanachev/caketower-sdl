#ifndef _GAME_H_
#define _GAME_H_

#include <SDL.h>
#include "sprite.h"
#include "textfield.h"

#include "list.h"

// buttons
typedef struct {
	sprite* play;
} game_buttons;

extern game_buttons* b;


// game objects
typedef struct {
	sprite* bg1;
	sprite* bg2;
	sprite* baker;
	sprite* plate;
	sprite* logo;
	sprite* table;

	sprite* arm;	
	sprite* gameover;
	sprite* tutorial;
	sprite* timeup;
	sprite* clock;
	
	textfield* timeField;
	textfield* scoreField;
} game_environment;

extern game_environment* env;

// game vars
typedef struct {
	int playing;
	int gameOver;
	int dropping;
	int score;
	double speed;
	int dir;
	int dt;
	int cakesAvailable;
	int cakeAmount;
	int cakeSize;
	int dropSpeed;
	int timeLeft;
	int totalHeight;
	
	list_t* cakesToDrop;
	list_t* cakes; 
	list_t* falling; 
	list_t* clouds; 
} game;

extern game* g;

void game_freeSpriteList(list_t**);

void game_addCloud();

void game_showMenu();
void game_hideMenu();

void game_showGame();
void game_startGame();
void game_finishGame();
void game_hideGame();

void game_reset();

void game_makeFallCheck();
void game_makeNewCake();

void game_init();
void game_update();
void game_free();
int game_handleEvent(SDL_Event* e);
#endif

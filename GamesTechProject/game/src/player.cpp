#include "player.h"

player::player()
{}
player::~player()
{}
//Initializes player with 100 health and 30 ammo
void player::init()
{
	health = 100;
	ammoCount = 30;
	oxygen = 100;
	oxygenCounter;
}


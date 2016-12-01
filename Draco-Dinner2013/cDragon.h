/*
=================
cRocket.h
- Header file for class definition - SPECIFICATION
- Header file for the Dragon class which is a child of cSprite class
=================
*/
#ifndef _CROCKET_H
#define _CROCKET_H
#include "cSprite.h"

class cDragon : public cSprite
{
private:
	SDL_Point dragonVelocity;

public:
	cDragon();
	void update(double deltaTime);		// Dragon Character update method
	void setDragonVelocity(SDL_Point dragonVel);   // Sets the velocity for the dragon
	SDL_Point getDragonVelocity();				 // Gets the  velocity of dragon
};
#endif
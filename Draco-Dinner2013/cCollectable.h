/*
=================
cAsteroid.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CASTEROID_H
#define _CASTEROID_H
#include "cSprite.h"

class cCollectable : public cSprite
{
		
private:
	SDL_Point collectableVelocity;
public:
	cCollectable();
	void update(double deltaTime);		// Collectable update method
	void setCollectableVelocity(SDL_Point collectableVel);   // Sets the velocity for the Collectable
	SDL_Point getCollectableVelocity(); // Gets the Collectable velocity
};
#endif
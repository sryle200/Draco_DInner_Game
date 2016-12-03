/*
=================
cAsteroid.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cCollectable.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cCollectable::cCollectable() : cSprite()
{
	this->collectableVelocity = { 0.0, 0.0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cCollectable::update(double deltaTime)
{

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->getSpriteTranslation().x * deltaTime;
	currentSpritePos.y += this->getSpriteTranslation().y * 1 + deltaTime;

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	cout << "Collectable position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
  Sets the velocity for the Collectable
=================================================================
*/
void cCollectable::setCollectableVelocity(SDL_Point CollectableVel)
{
	collectableVelocity = CollectableVel;
}
/*
=================================================================
  Gets the Collectable velocity
=================================================================
*/
SDL_Point cCollectable::getCollectableVelocity()
{
	return collectableVelocity;
}

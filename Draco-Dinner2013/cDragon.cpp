/*
=================
cRocket.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cDragon.h";

/*
=================================================================
Defualt Constructor
=================================================================
*/
cDragon::cDragon() : cSprite()
{
	this->dragonVelocity = { 0.0f, 0.0f };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cDragon::update(double deltaTime)
{

	FPoint direction = { 0.0f, 0.0f };
	direction.X = (sin(this->getSpriteRotAngle()*PI / 180));
	direction.Y = -(cos(this->getSpriteRotAngle()*PI / 180));

	direction.X *= this->getSpriteTranslation().x;
	direction.Y *= this->getSpriteTranslation().y;

	this->dragonVelocity.x = this->dragonVelocity.x + direction.X;
	this->dragonVelocity.y = this->dragonVelocity.y + direction.Y;

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->dragonVelocity.x * deltaTime;
	currentSpritePos.y += this->dragonVelocity.y * deltaTime;

	this->dragonVelocity.x *= 0.95;
	this->dragonVelocity.y *= 0.95;
	
	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y  });
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
Sets the velocity for the rocket
=================================================================
*/
void cDragon::setDragonVelocity(SDL_Point dragonVel)
{
	dragonVelocity = dragonVel;
}
/*
=================================================================
Gets the rocket velocity
=================================================================
*/
SDL_Point cDragon::getDragonVelocity()
{
	return dragonVelocity;
}

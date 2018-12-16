#include "Steve.h"



void Steve::Update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		ApplyForce(vector3(-1, 0, 0));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		ApplyForce(vector3(1, 0, 0));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		ApplyForce(vector3(0, 0, -1));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		ApplyForce(vector3(0, 0, 1));
	}
	DynamicEntity::Update();
}

Steve::Steve(vector3 pos, vector3 vel, String a_sFileName, String a_sUniqueID) : DynamicEntity(pos, vel, a_sFileName, a_sUniqueID)
{
}


Steve::~Steve()
{
}

#include "Object.h"
using namespace Engine;
using namespace sf;

Engine::Object::Object(sf::Vector2f pos, std::string n)
{
	position = pos;
	IsActive = true;
	type = ObjectType::OEntity;
	name = n;
}

Engine::Object::Object(sf::Vector2f pos, ObjectType t, std::string n)
{
	position = pos;
	type = t;
	name = n;
	IsActive = true;
}



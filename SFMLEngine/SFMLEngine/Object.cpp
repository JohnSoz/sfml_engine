#include "Object.h"
using namespace Engine;
using namespace sf;

Engine::Object::Object(sf::Vector2f pos, std::string n)
{
	position = pos;
	IsActive = true;
	type = ObjectType::None;
	name = n;
}

Engine::Object::Object(std::string Name)
{
	name = Name;
	type = ObjectType::None;
}

Engine::Object::Object(sf::Vector2f pos, ObjectType t, std::string n)
{
	position = pos;
	type = t;
	name = n;
	IsActive = true;
}



#include "Object.h"
using namespace Engine;
using namespace sf;

Engine::Object::Object(sf::Vector2f pos, std::string n)
{
	position = pos;
	IsActive = true;
	type = ObjectType::None;
	name = n;
	dw_o.set(this);
}


Engine::Object::Object()
{
	type = ObjectType::None;
	dw_o.set(this);
}

Engine::Object::Object(std::string Name)
{
	name = Name;
	type = ObjectType::None;
	dw_o.set(this);
}

Engine::Object::Object(sf::Vector2f pos, ObjectType t, std::string n)
{
	position = pos;
	type = t;
	name = n;
	IsActive = true;
	dw_o.set(this);
}

bool Engine::operator==(const Object & obj, const Object & obj2)
{
	return obj.name == obj2.name;
}

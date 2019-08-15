#include "Object.h"
using namespace Engine;
using namespace sf;

Engine::Object::Object(sf::Vector2f pos, std::string n) :
	name(std::move(n)), position(pos), type(ObjectType::None)
{
	IsActive = true;
}


Engine::Object::Object(sf::Image img, sf::Vector2f pos, ObjectType t, std::string n) :
	name(std::move(n)), type(t), position(pos)
{
	IsActive = true;
	texture.loadFromImage(img);
	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setPosition(position);
}


void Engine::Object::setTexture(const sf::Texture& tex)
{
	texture = tex;
	sprite.setTexture(texture);
}

Engine::Object::Object() :
	type(ObjectType::None)
{
	IsActive = true;
}

Engine::Object::Object(std::string n) :
	name(std::move(n)), type(ObjectType::None)
{
	IsActive = false;
}

Engine::Object::Object(sf::Vector2f pos, ObjectType t, std::string n) :
	name(std::move(n)), position(pos), type(t)
{
	IsActive = true;
}

bool Engine::operator==(const Object& obj, const Object& obj2)
{
	return obj.name == obj2.name;
}

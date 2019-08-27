#include "Object.h"
using namespace Engine;
using namespace sf;


Engine::Object::Object(sf::Image img, sf::Vector2f pos, std::string n) :
	BaseObject(n), position(pos)
{
	texture.loadFromImage(img);
	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setPosition(position);
	setType(OBject);
}

void Engine::Object::setTexture(const sf::Texture& tex)
{
	texture = tex;
	sprite.setTexture(texture);
}

Engine::Object::Object(sf::Vector2f pos, std::string n) :
	BaseObject(n), position(pos)
{
	setType(OBject);
}



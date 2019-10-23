#include "Object.h"
using namespace Engine;
using namespace sf;


Engine::Object::Object(sf::Image img, sf::Vector2f _position, std::string _name) :
	BaseObject(_name), position(_position)
{
	texture.loadFromImage(img);
	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setPosition(position);
	setType(ObjectType::OBject);
}

void Engine::Object::setTexture(const sf::Texture& _texture)
{
	texture = _texture;
	sprite.setTexture(texture);
}

Engine::Object::Object(sf::Vector2f _position, std::string _name) :
	BaseObject(_name), position(_position)
{
	setType(ObjectType::OBject);
}



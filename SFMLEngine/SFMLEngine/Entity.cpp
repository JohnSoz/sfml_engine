#include "Entity.h"
 
Engine::Entity::Entity(sf::Vector2f POSITION, std::string NAME) : Object(POSITION, NAME)
{
	type = ObjectType::OEntity;
}

Engine::Entity::Entity(sf::Image & IMAGE, sf::Vector2f POSITION, std::string NAME) : Object(POSITION, NAME)
{
	type = ObjectType::OEntity;
	texture.loadFromImage(IMAGE);
	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setPosition(position);
}

Engine::Entity::Entity(sf::Image & i, sf::IntRect r, sf::Vector2f pos, std::string name) : Object(pos, name)
{
	type = ObjectType::OEntity;
	localRectangle = r;
	position = pos;
	globalRectangle = sf::FloatRect(position.x, position.y, r.width, r.height);
	texture.loadFromImage(i);
	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setPosition(position);
	sprite.setTextureRect(r);
}

Engine::Entity::~Entity()
{
}


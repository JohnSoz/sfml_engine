#include "Entity.h"

Engine::Entity::Entity(sf::Vector2f POSITION, std::string NAME) : Object(POSITION, NAME)
{
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
}

Engine::Entity::~Entity()
{
}


#include "Object.h"
using namespace Engine;

Engine::Entity::Entity(Vector2D POSITION, std::string NAME) : position(POSITION), name(NAME)
{
}

Engine::Entity::Entity(sf::Image & IMAGE, Vector2D POSITION, std::string NAME) : position(POSITION), name(NAME)
{
	texture.loadFromImage(IMAGE);
	sprite.setTexture(texture);
	sprite.setPosition(position.GetSfmlVector());
}

Engine::Entity::Entity(sf::Image & i, sf::IntRect r, Vector2D pos, std::string name)
{
}

Engine::Entity::~Entity()
{
}

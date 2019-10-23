#include "Pawn.h"

Engine::Pawn::Pawn(std::string _name, std::string_view _pathToTexture, std::string_view _animation, sf::RenderWindow& _window, Level& _lvl, sf::Vector2f _position)
	:BaseObject(_name), ColliderSprite(_pathToTexture)
{
	window = &_window;
	levelObjects = _lvl.GetAllObjects();
	animManager.loadAnimation_x(_animation, *window);
	setPosition(_position);
}

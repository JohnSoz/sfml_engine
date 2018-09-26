#pragma once
#include <SFML/Graphics/View.hpp>

class Camera
{
private:
	sf::Vector2f center;
	sf::Vector2f position;
	sf::IntRect size;
	sf::View camera;
public:
	Camera() { position = center = { 0,0 }; }

	~Camera();
};


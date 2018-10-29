#include "Camera.h"
using namespace Engine;

void Engine::Camera::moveToPoint(sf::Vector2f point, sf::Vector2f offset)
{
	float distance = sqrt(pow(point.x - camera.getCenter().x, 2) + pow(point.y - camera.getCenter().y, 2));
	if (distance > 1)
	{
		move = true;
		camera.move(sf::Vector2f(offset.x * (point.x - camera.getCenter().x) / distance, offset.y  * (point.y - camera.getCenter().y) / distance));
	}
	else
		move = false;
}


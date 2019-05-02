#include "Camera.h"
using namespace Engine;

void Engine::Camera::moveToPoint(sf::Vector2f point, float t, sf::Vector2f offset)
{
	float distance = sqrt(pow(point.x - camera.getCenter().x, 2) + pow(point.y - camera.getCenter().y, 2));
	if (distance > 4)
	{
		move = true;
		camera.move(sf::Vector2f(offset.x * (point.x - camera.getCenter().x) / distance, offset.y * (point.y - camera.getCenter().y) / distance));
	}
	else
		move = false;
	//camera.setCenter(560, 300);
}

void Engine::Camera::setPosition(float x, float y)
{
	camera.setCenter(x, y);
}

void Engine::Camera::setViewPort(float w, float h)
{
	camera.reset({ 0,0, w, h });
}


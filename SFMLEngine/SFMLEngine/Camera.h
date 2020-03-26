#pragma once
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics.hpp>
#include "Math.h"

namespace Engine
{
	class Camera
	{
	private:
		sf::FloatRect viewPort;
		sf::View      camera;
		sf::Vector2f  defaultCenter;

		bool move = false;
	public:
		Camera()
		{
			camera.reset(sf::FloatRect(0, 0, 1000, 281 * 2));
			camera.setCenter(300, 600);
			defaultCenter = camera.getCenter();
		}
		Camera(sf::Vector2f pos, sf::IntRect s)
		{
			camera.reset(sf::FloatRect(0, 0, 1000, 281 * 2));
			defaultCenter = camera.getCenter();
		}
		~Camera() {}

		bool isMove() { return move; }
		sf::Vector2f getPositionOffset()
		{
			return VectorAbs(defaultCenter - camera.getCenter());
		}
		void moveToPoint(sf::Vector2f point, float time,sf::Vector2f offset = { 2.f, 2.f });
		void setPosition(float x, float y);
		void setViewPort(float w, float h);
		const sf::View & getView() { return camera; }
	};
}


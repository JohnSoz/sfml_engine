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
		sf::View camera;
		sf::Vector2f defaultCenter;
		Engine::Quad q;
		bool move = false;
	public:
		Camera()
		{
			camera.reset(sf::FloatRect(0, 0, 1000, 563));//1000//563
			camera.setCenter(300, 600); 
			defaultCenter = camera.getCenter();
		}
		Camera(sf::Vector2f pos, sf::IntRect s)
		{
			camera.reset(sf::FloatRect(0, 0, 1000, 563));
			defaultCenter = camera.getCenter();
		}
		~Camera() {}

		bool isMove() { return move; }
		sf::Vector2f getPositionOffset() 
		{
			return VectorAbs(defaultCenter - camera.getCenter());
		}
		void moveToPoint(sf::Vector2f point, sf::RenderWindow& window, sf::Vector2f offset = { 0.4f, 0.4f });
		const sf::View& getView() { return camera; }
	};
}


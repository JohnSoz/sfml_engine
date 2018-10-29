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
		Engine::Quad q;
		bool move = false;
	public:
		Camera() 
		{
			camera.reset(sf::FloatRect(0, 0, 1000, 563));
		}
		Camera(sf::Vector2f pos, sf::IntRect s)
		{
			camera.reset(sf::FloatRect(0, 0, 1000, 563));
		}
		~Camera() {}

		bool isMove() { return move; }
		void moveToPoint(sf::Vector2f point, sf::Vector2f offset = { 0.4f, 0.4f });
		const sf::View& getView() { return camera; }
	};
}


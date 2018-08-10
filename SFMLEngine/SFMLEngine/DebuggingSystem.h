#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#define Debug 1;

namespace Engine
{
	namespace Debugging
	{
		class DebuggingSystem
		{
		private:
			static sf::RenderWindow* window;
			std::vector<Entity*> entites;
		public:
			static void setWindow(sf::RenderWindow& w)
			{
				window = &w;
			}
			void draw()
			{
				std::vector<sf::Vertex*> rects;
				for (auto &i : entites)
				{
					auto x = i->getPos().x;
					auto y = i->getPos().y;
					auto w = i->getRect().w;
					auto h = i->getRect().h;
					sf::Vertex vertices[5] =
					{
						sf::Vertex(sf::Vector2f(x, y),sf::Color::Red),
						sf::Vertex(sf::Vector2f(x + w, y),sf::Color::Red),
						sf::Vertex(sf::Vector2f(x + w, y + h),sf::Color::Red),
						sf::Vertex(sf::Vector2f(x, y + h),sf::Color::Red),
						sf::Vertex(sf::Vector2f(x, y),sf::Color::Red)
					};
					rects.push_back(vertices);
				}
				for (auto &i : rects)
					window->draw(i, 5, sf::LinesStrip);
			}
			void pushEntites(Entity& e)
			{
				entites.push_back(&e);
			}
			DebuggingSystem() = default;
			~DebuggingSystem() = default;
		};
	}
}


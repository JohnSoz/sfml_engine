#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#define Debug 0;

namespace Engine
{
	namespace Debugging
	{
		class DebuggingSystem
		{
		private:
			static sf::RenderWindow* window;
			std::vector<std::pair<Rectangle, Vector2D>> entites;
		public:
			static void setWindow(sf::RenderWindow& w)
			{
				window = &w;
			}
			void draw();
			void pushEntites(Entity& e)
			{
				entites.push_back(std::make_pair(e.getRect(), e.getPos()));
			}
			DebuggingSystem() = default;
			~DebuggingSystem() = default;
		};
	}
}


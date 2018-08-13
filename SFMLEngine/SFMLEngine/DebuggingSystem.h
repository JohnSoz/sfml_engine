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
			std::vector<std::pair<Rectangle*, Vector2D*>> entites;
			std::vector<sf::FloatRect*> obj;
		public:
			static void setWindow(sf::RenderWindow& w)
			{
				window = &w;
			}
			void draw();
			void pushEntites(Entity& e)
			{
				entites.push_back(std::make_pair(&e.getRect(), &e.getPos()));
			}
			void levelObjects(std::vector<ObjectLevel> objs)
			{
				for (auto& i : objs)
					obj.push_back(&i.rect);
			}
			DebuggingSystem() = default;
			~DebuggingSystem() = default;
		};
	}
}


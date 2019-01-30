#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"

namespace Engine
{
	class Collider
	{
	private:
		sf::FloatRect rect;
		float widht, height;
		sf::Transformable transform;
	public:
		Collider() = delete;
		Collider(float x, float y, float w, float h)
			: rect(x, y, x + w, y + h)
		{
		}

		void rotate(float angle)
		{
			rect = sf::Transform().rotate(angle).transformRect(rect);
		}
	};
} // namespace Engine
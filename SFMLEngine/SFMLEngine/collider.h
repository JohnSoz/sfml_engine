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
	public:
		Collider() = delete;
		Collider(float x, float y, float w, float h)
			: rect(x, y, x + w, y + h)
		{
			widht = w;
			height = h;
		}
		void rotate(float angle, bool rotate—enter = false)
		{
			sf::Transformable t;
			if (rotate—enter)
				t.setOrigin(widht / 2, height / 2);
			t.rotate(angle);
			rect = t.getTransform().transformRect(rect);
		}
		void setPosition(sf::Vector2f pos)
		{
			rect = sf::FloatRect(pos.x, pos.y, pos.x + widht, pos.y + height);
		}
		sf::Vector2f checkClashes(const Collider& other)
		{
			auto this_collider = Rectangle::fromSfmlRect(rect);
			auto other_collider = Rectangle::fromSfmlRect(other.rect);
			return Rectangle::GetIntersectionDepth(this_collider, other_collider).GetSfmlVector();
		}
	};
} // namespace Engine
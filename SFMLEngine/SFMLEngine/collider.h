#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include <utility>
#include <MetaStuff/Meta.h>
namespace Engine
{
	class Collider
	{
	private:
		sf::FloatRect rect;
		sf::Vector2f origin;
		float widht, height;
		float scale;
		auto makeScaleQuad() const
		{
			auto pos = sf::Vector2f(rect.left, rect.top);
			auto r1 = sf::FloatRect(pos.x, pos.y, pos.x + widht * scale, pos.y + height * scale);
			auto r2 = sf::FloatRect(r1.width, pos.y, pos.x, r1.height);
			return std::make_pair(r1, r2);
		}
		sf::Vector2f getPos(bool withOrigin = false)
		{
			if (withOrigin)
				return { rect.left - origin.x,rect.top - origin.y };
			else
				return { rect.left,rect.top };
		}
	public:
		Collider() { widht = scale = height = 0; }

		Collider(float x, float y, float w, float h, float sc)
			: rect(x, y, x + w, y + h)
		{
			widht = w;
			height = h;
			scale = sc;
		}
		Collider(sf::Vector2f pos, sf::Vector2f size, float sc = 1.f)
			: Collider(pos.x, pos.y, size.x, size.y, sc)
		{
		}
		sf::FloatRect rotate(float angle, bool rotate—enter = false)
		{
			sf::Transformable t;
			if (rotate—enter)
				t.setOrigin(widht / 2, height / 2);
			t.rotate(angle);
			rect = t.getTransform().transformRect(rect);
			return rect;
		}
		void setPosition(sf::Vector2f pos)
		{
			rect = sf::FloatRect(pos.x, pos.y, pos.x + widht, pos.y + height);
		}
		void setScale(float sc) { scale = sc; }
		void setCenter(sf::Vector2f c) { origin = c; }
		void setSize(sf::Vector2f size)
		{
			auto deltaW = size.x - widht;
			auto deltaH = size.y - height;
			widht = size.x;
			height = size.y;
			rect.width += deltaW;
			rect.height += deltaH;
		}
		sf::Vector2f checkClashes(const Collider& other)
		{
			auto this_collider = rect;
			auto other_collider = other.rect;
			return GetIntersectionDepth(this_collider, other_collider);
		}
		float distance(const Collider& other)
		{
			float dist = 0;
			auto this_pos = sf::Vector2f(rect.left, rect.top);
			auto other_pos = sf::Vector2f(other.rect.left, other.rect.top);

			if (this_pos.x < other_pos.x)
				this_pos.x += widht;
			else
				other_pos.x += other.widht;
			if (this_pos.y < other_pos.y)
				this_pos.y = other_pos.y;
			else
				other_pos.y = this_pos.y;
			dist = Distance(this_pos, other_pos);
			return dist;
		}
		auto getQuad(bool witchScale = false) const
		{
			if (witchScale)
				return makeScaleQuad();
			else
				return std::make_pair(rect, sf::FloatRect(rect.left + widht, rect.top, rect.left, rect.top + height));
		}
		friend auto meta::registerMembers<Collider>();
	};
} // namespace Engine
namespace meta
{
	template <>
	inline auto registerMembers<Engine::Collider>()
	{
		return members(
			member("origin", &Engine::Collider::origin),
			member("widht", &Engine::Collider::widht),
			member("height", &Engine::Collider::height),
			member("rect", &Engine::Collider::rect),
			member("scale", &Engine::Collider::scale)
		);
	}
}
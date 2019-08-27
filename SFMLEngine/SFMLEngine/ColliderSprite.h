#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
namespace Engine
{
	class ColliderSprite : public sf::Sprite
	{
	private:
		std::string pathToTex;
		sf::Texture t;
		sf::FloatRect rect;
		auto makeScaleQuad() const
		{
			auto pos = sf::Vector2f(rect.left, rect.top);
			auto size = sf::Vector2f(getTextureRect().width, getTextureRect().height);
			auto r1 = sf::FloatRect(pos.x, pos.y, pos.x + size.x * getScale().x, pos.y + size.y * getScale().y);
			auto r2 = sf::FloatRect(r1.width, pos.y, pos.x, r1.height);
			return std::make_pair(r1, r2);
		}
		void updateRect()
		{
			auto pos = getPosition();
			auto size = sf::Vector2f(getTextureRect().width, getTextureRect().height);
			rect.left = pos.x;
			rect.top = pos.y;
			rect.width = pos.x + size.x;
			rect.height = pos.y + size.y;
		}
	public:
		ColliderSprite(std::string_view path)
		{
			pathToTex = path;
			t.loadFromFile(pathToTex);
			setTexture(t);
		}
		ColliderSprite(std::string_view path, sf::IntRect rect) : ColliderSprite(path)
		{
			setTextureRect(rect);
		}
		sf::Vector2f checkClashes(const ColliderSprite& other)
		{
			auto this_collider = rect;
			auto other_collider = other.rect;
			return GetIntersectionDepth(this_collider, other_collider);
		}
		void setRect(sf::IntRect rectA)
		{
			setTextureRect(rectA);
			auto deltaW = rectA.width - getTextureRect().width;
			auto deltaH = rectA.height - getTextureRect().height;
			rect.width += deltaW;
			rect.height += deltaH;
		}
		float distance(const ColliderSprite& other)
		{
			float dist = 0;
			auto this_pos = sf::Vector2f(rect.left, rect.top);
			auto other_pos = sf::Vector2f(other.rect.left, other.rect.top);

			if (this_pos.x < other_pos.x)
				this_pos.x += getTextureRect().width;
			else
				other_pos.x += other.getTextureRect().width;
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
				return std::make_pair(rect, sf::FloatRect(rect.left + getTextureRect().width, rect.top, rect.left, rect.top + getTextureRect().height));
		}
	};
}


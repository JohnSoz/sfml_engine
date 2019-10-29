#pragma once
#include "Math.h"
namespace Engine
{
	class Ray
	{
	private:
		sf::Vector2f start;
		sf::Vector2f end;
		sf::VertexArray line;
		float dist;
		float angle;

		void intersec(sf::Vector2f v1, sf::Vector2f v2);
	public:
		Ray() : line(sf::Lines, 2), dist(-1), angle(-1)
		{}

		Ray(sf::Vector2f _start, float _dist, float _angle);

		void setEnd(const sf::Vector2f& _end);
		void setStart(const sf::Vector2f& _start);
		void setDist(float _dist);
		void setAngle(float _angle);
		auto getEnd()  const { return end; }
		auto getStart() const { return start; }
		float hitDist() const { return Distance(start, end); }
		void update();

		std::pair<bool, sf::Vector2f> castTo(const Ray& ray);
		std::pair<bool, sf::Vector2f> castTo(const sf::FloatRect& obj);

		operator sf::Drawable& () { return line; }

	};
}
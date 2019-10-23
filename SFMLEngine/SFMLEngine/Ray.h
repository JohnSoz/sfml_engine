#pragma once
#include "Math.h"
namespace Engine
{
	class Ray
	{
	private:
		sf::Vector2f start;
		sf::VertexArray visualization;
		sf::Vector2f end;
		float dist;
		float angle;

	public:
		Ray(sf::Vector2f _start, float _dist, float _angle) : start(_start), dist(_dist), angle(_angle), visualization(sf::Lines, 2)
		{
			visualization[0].position = start;
			visualization[1].position = sf::Vector2f(start.x + dist, start.y);
			visualization[1].position = rotateLine(visualization[1].position, visualization[0].position, _angle);
			end = visualization[1].position;
		}

		Ray(sf::Vector2f start, sf::Vector2f end) {}

		void castTo(const sf::FloatRect& obj)
		{
			visualization[1].position = rotateLine(visualization[1].position, visualization[0].position, 0);
			end = visualization[1].position;
			bool collision = false;

			sf::Vector2f check = lineLineIntersection(start, end, sf::Vector2f(obj.left, obj.top), sf::Vector2f(obj.left + obj.width, obj.top));
			sf::Vector2f check2 = lineLineIntersection(start, end, sf::Vector2f(obj.left + obj.width, obj.top),
				sf::Vector2f(obj.left + obj.width, obj.top + obj.height));
			sf::Vector2f check3 = lineLineIntersection(start, end, sf::Vector2f(obj.left + obj.width, obj.top + obj.height),
				sf::Vector2f(obj.left, obj.top + obj.height));
			sf::Vector2f check4 = lineLineIntersection(start, end, sf::Vector2f(obj.left, obj.top + obj.height), sf::Vector2f(obj.left, obj.top));

			auto pointCheck = [&collision, &check, this](sf::Vector2f vec1, sf::Vector2f vec2)
			{
				if (thc(vec1, vec2, check) && start.x < vec1.x)
					if (Distance(start, check) < dist)
						collision = true;
			};
			pointCheck({ obj.left, obj.top }, { obj.left + obj.width, obj.top });
			pointCheck({ obj.left + obj.width, obj.top }, { obj.left + obj.width, obj.top + obj.height });
			pointCheck({ obj.left + obj.width, obj.top + obj.height }, { obj.left, obj.top + obj.height });
			pointCheck({ obj.left, obj.top + obj.height }, { obj.left, obj.top });
			if (collision)
			{
				visualization[0].color = sf::Color::Red;
				visualization[1].color = sf::Color::Green;
			}
			else
			{
				visualization[0].color = sf::Color::White;
				visualization[1].color = sf::Color::White;
			}
		}

		void castTo(const Ray& other)
		{
			sf::Vector2f check = lineLineIntersection(start, end, other.start, other.end);
			if (Distance(start, check) < dist)
				if (thc(other.start, other.end, check))
				{
					end = rotateLine(check, start, angle);
				}
				else
				{
					float _angle = angleVector(end, start);
					sf::Vector2f z = { start.x + dist, start.y };
					end = rotateLine(z, start, _angle);
				}
			visualization[1].position = end;
		}
	};
}
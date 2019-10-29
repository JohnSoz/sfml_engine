#include "Ray.h"
using namespace Engine;
void Ray::update()
{
	line[0].position = start;
	line[1].position = sf::Vector2f(start.x + dist, start.y);
	line[1].color = sf::Color(255, 0, 0);
	end = rotateLine(line[1].position, line[0].position, angle);
	line[1].position = end;
}

Ray::Ray(sf::Vector2f _start, float _dist, float _angle) :
	start(_start), dist(_dist), angle(_angle), line(sf::Lines, 2)
{
	update();
	line[1].position = rotateLine(line[1].position, line[0].position, _angle);
	end = line[1].position;
}

void Ray::setStart(const sf::Vector2f& _start)
{
	update();
	Ray::start = _start;
}

void Ray::setEnd(const sf::Vector2f& _end)
{
	update();
	Ray::end = _end;
}

void Ray::setDist(float _dist)
{
	update();
	Ray::dist = _dist;
}

void Ray::setAngle(float _angle)
{
	update();
	Ray::angle = _angle;
}

std::pair<bool, sf::Vector2f> Ray::castTo(const sf::FloatRect& obj)
{
	intersec(sf::Vector2f(obj.left, obj.top), sf::Vector2f(obj.left + obj.width, obj.top));
	intersec(sf::Vector2f(obj.left + obj.width, obj.top),
		sf::Vector2f(obj.left + obj.width, obj.top + obj.height));
	intersec(sf::Vector2f(obj.left + obj.width, obj.top + obj.height),
		sf::Vector2f(obj.left, obj.top + obj.height));
	intersec(sf::Vector2f(obj.left, obj.top + obj.height), sf::Vector2f(obj.left, obj.top));
	line[1].position = end;
	if (Distance(start, end) < dist)
		return { true,end };
	else
		return { false,{-1,-1} };
}

std::pair<bool, sf::Vector2f> Ray::castTo(const Ray& obj)
{
	intersec(obj.start, obj.end);
	line[1].position = end;
	return { true, {0,0} };
}

void Ray::intersec(sf::Vector2f v1, sf::Vector2f v2)
{
	if (auto check = _checkIntersects(start, end, v1, v2); check != sf::Vector2f(-1, -1))
	{
		if (Distance(start, check) < dist)
			if (thc(v1, v2, check))
			{
				end = rotateLine(check, start, angle);
			}
			else
			{
				float _angle = angleVector(end, start);
				sf::Vector2f z = { start.x + dist, start.y };
				end = rotateLine(z, start, _angle);
			}
	}
}
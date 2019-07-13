#pragma once
#ifndef MATH
#define MATH
#include <iostream>
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <array>
namespace Collision
{
	struct rectangle
	{
		sf::Vector2f vertex[4];
	};

	class SAT
	{
	public:
		bool collision(sf::Sprite* s1, sf::Sprite* s2);

	private:
		void project(sf::Vector2f& axis, rectangle* _rectangle, float& min, float& max);
		void normalize(sf::Vector2f& vector);
		float dot(sf::Vector2f& vector1, sf::Vector2f& vector2);
		float distance(float minA, float maxA, float minB, float maxB);
	};

	bool PixelPerfectTest(const sf::Sprite& Object1, const sf::Sprite& Object2, sf::Uint8 AlphaLimit = 0);

	bool CreateTextureAndBitmask(sf::Texture& LoadInto, const std::string& Filename);

	bool CircleTest(const sf::Sprite& Object1, const sf::Sprite& Object2);

	bool BoundingBoxTest(const sf::Sprite& Object1, const sf::Sprite& Object2);
} // namespace Collision

namespace Engine
{
	sf::Vector2f VectorAbs(sf::Vector2f vec);


	inline float Normalize(sf::Vector2f v)
	{
		return sqrt(v.x * v.x + v.y * v.y);
	}

	inline bool operator<(sf::Vector2f v1, sf::Vector2f v2)
	{
		return (v1.x < v2.x) && (v1.y < v2.y);
	}

	inline sf::Vector2f operator-(sf::Vector2f v1, sf::Vector2f v2)
	{
		return { v1.x - v2.x, v1.y - v2.y };
	}

	inline float lenght(sf::Vector2f v1)
	{
		return sqrt(v1.x * v1.x + v1.y * v1.y);
	}

	inline float dot(sf::Vector2f v1)
	{
		return v1.x * v1.x + v1.y * v1.y;
	}

	inline sf::Vector2f perp(sf::Vector2f v1)
	{
		return { v1.y,-v1.x };
	}

	inline float Distance(const sf::Vector2f& v2, const sf::Vector2f& v1)
	{
		return sqrtf(pow((v2.x - v1.x), 2) + pow((v2.y - v1.y), 2));
	}

	inline sf::Vector2f raycast(sf::Vector2f start, sf::Vector2f end, std::vector<sf::FloatRect> objects, sf::Vector2f size)
	{
		sf::Vector2f u = end - start;
		u /= Normalize(u);
		sf::Vector2f prevPos = start;

		while (sf::FloatRect(0, 0, size.x, size.y).contains(start))
		{
			sf::FloatRect rect(start.x, start.y, .1f, .1f);
			for (auto s : objects)
			{
				if (s.intersects(rect))
				{
					float distance = Distance(prevPos, { s.left, s.top });
					return start;
				}
			}
			start += u;
		}
		return start;
	}


	class Vector2D
	{
	public:
		Vector2D(float x = 0, float y = 0);
		Vector2D(Vector2D&&) = default;
		Vector2D(const Vector2D&) = default;
		~Vector2D() {};

		void Rotate(const float angle);
		float Magnitude() const;
		float Normalize();
		float DotProduct(const Vector2D& v2) const;
		float CrossProduct(const Vector2D& v2) const;
		static Vector2D Zero();
		static float Distance(const Vector2D& v1, const Vector2D& v2);

		sf::Vector2f GetSfmlVector();
		static Vector2D FromSfmlVec(sf::Vector2f);

		Vector2D& operator=(const Vector2D& v2);
		Vector2D& operator+=(const Vector2D& v2);
		Vector2D& operator-=(const Vector2D& v2);
		Vector2D& operator*=(const float scalar);
		Vector2D& operator/=(const float scalar);

		const Vector2D operator+(const Vector2D& v2) const;
		const Vector2D operator-(const Vector2D& v2) const;
		const Vector2D operator*(const float scalar) const;
		const Vector2D operator/(const float scalar) const;

		bool operator==(const Vector2D& v2) const;
		bool operator>(const Vector2D& v2) const;
		bool operator<(const Vector2D& v2) const;
		bool operator>=(const Vector2D& v2) const;
		bool operator<=(const Vector2D& v2) const;
		bool operator!=(const Vector2D& v2) const;

		friend std::ostream& operator<<(std::ostream& os, const Vector2D& dt);

	public:
		float x, y;
	};

	class Quad
	{
	public:
		sf::FloatRect leftRect;
		sf::FloatRect rightRect;
		Quad(float left_x, float left_y, float left_down_x, float left_down_y, float right_x, float right_y, float right_down_x, float right_down_y)
			: leftRect(left_x, left_y, left_down_x, left_down_y),
			rightRect(right_x, right_y, right_down_x, right_down_y)
		{
		}
		Quad() = default;
	};

	class Rectangle
	{
	public:
		Rectangle(double x = 0, double y = 0, double w = 0, double h = 0);
		~Rectangle(void);;

		float Left() const;
		float Right() const;
		float Top() const;
		float Bottom() const;

		float getX() const { return x; }
		float getY() const { return y; }
		float getW() const { return w; }
		float getH() const { return h; }

		bool Contains(Vector2D& vVec) const;
		bool Contains(double x, double y) const;

		sf::IntRect getSfmlRect_i();
		sf::FloatRect* getSfmlRect_f();
		static Rectangle fromSfmlRect(sf::FloatRect);

		static Rectangle Empty();

		Rectangle getIntersectionRect(const Rectangle& rect);
		static Vector2D GetIntersectionDepth(const Rectangle& rectA, const Rectangle& rectB);
		static Vector2D GetIntersectionDepth2(const Rectangle& rectA, const Rectangle& rectB);
		static Vector2D GetBottomCenter(const Rectangle& rect);
		static Vector2D GetCenter(const Rectangle& rect);
		static float GetDistance(const Rectangle& rectA, const Rectangle& rectB);
		static Vector2D GetDirection(const Rectangle& rectA, const Rectangle& rectB);

		Rectangle& operator=(const Rectangle& r2);

		bool operator==(const Rectangle& r2) const;
		bool operator!=(const Rectangle& r2) const;

	private:
		float x, y, w, h;
	};
} // namespace Engine
#endif
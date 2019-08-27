#pragma once
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
	template<class T, class T2>
	bool operator<(const sf::Vector2<T>& v1, const sf::Vector2<T2>& v2)
	{
		return (v1.x < v2.x) && (v1.y < v2.y);
	}
	template<class T, class T2>
	auto operator*(const sf::Vector2<T>& v1, const sf::Vector2<T2>& v2) -> sf::Vector2<decltype(v1.x* v2.x)>
	{
		return { v1.x * v2.x, v1.y * v2.y };
	}

	template<class T, class T2>
	sf::Vector2<T2> operator+(const sf::Vector2<T>& rect, T2 value)
	{
		return { rect.x + value, rect.y + value };
	}

	template<class T, class T2>
	sf::Rect<T2> operator*(const sf::Rect<T>& rect, T2 value)
	{
		return { rect.left * value, rect.top * value,rect.width * value, rect.height * value };
	}

	inline sf::Vector2f operator-(sf::Vector2f v1, sf::Vector2f v2)
	{
		return { v1.x - v2.x, v1.y - v2.y };
	}

	inline float lenght(sf::Vector2f v1)
	{
		return sqrt(v1.x * v1.x + v1.y * v1.y);
	}

	template<class T>
	sf::Vector2<T> rotateBy(const sf::Vector2<T>& vec, float degrees, const sf::Vector2<T>& center)
	{
		degrees *= 3.14159f / 180.f;
		const float cs = cos(degrees);
		const float sn = sin(degrees);

		vec.x -= center.x;
		vec.y -= center.y;

		T nx = (T)(vec.x * cs - vec.y * sn);
		T ny = (T)(vec.x * sn + vec.y * cs);

		vec.x = nx;
		vec.y = ny;

		vec.x += center.x;
		vec.y += center.y;
		return vec;
	}

	template<class T>
	float getAngle(const sf::Vector2<T>& vec)
	{
		if (vec.y == 0)
			return vec.x < 0 ? 180 : 0;
		else if (vec.x == 0)
			return vec.y < 0 ? 270 : 90;

		if (vec.y > 0)
			if (vec.x > 0)
				return atan(vec.y / vec.x) * 180.f / 3.14159f;
			else
				return 180.0 - atan(vec.y / -vec.x) * 180.f / 3.14159f;
		else
			if (vec.x > 0)
				return 360.0 - atan(-vec.y / vec.x) * 180.f / 3.14159f;
			else
				return 180.0 + atan(-vec.y / -vec.x) * 180.f / 3.14159f;
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
	template<class T, class T2>
	inline auto GetIntersectionDepth(const sf::Rect<T>& rectA, const sf::Rect<T2>& rectB)
	{
		using result_type = std::common_type_t<T, T2>;
		float halfWidthA = rectA.width / 2.0f;
		float halfHeightA = rectA.height / 2.0f;
		float halfWidthB = rectB.width / 2.0f;
		float halfHeightB = rectB.height / 2.0f;

		// ÷ентр.
		sf::Vector2<result_type>  centerA(rectA.left + halfWidthA, rectA.top + halfHeightA);
		sf::Vector2<result_type>  centerB(rectB.left + halfWidthB, rectB.top + halfHeightB);

		// ¬ычислить текущие и минимальные непересекающиес€ рассто€ни€ между центрами.
		float distanceX = centerA.x - centerB.x;
		float distanceY = centerA.y - centerB.y;
		float minDistanceX = halfWidthA + halfWidthB;
		float minDistanceY = halfHeightA + halfHeightB;

		// ≈сли мы вообще не пересекаемс€,  (0, 0).
		if (abs(distanceX) >= minDistanceX || abs(distanceY) >= minDistanceY)
			return sf::Vector2<result_type>();

		// ¬ычисление и возврат глубины пересечени€.
		float depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
		float depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;
		if (abs(depthX) > abs(depthY))
			depthX = 0;
		else
			depthY = 0;
		return sf::Vector2<result_type>(depthX, depthY);
	}
	template<class T, class T2>
	inline auto GetIntersectionDepth2(const sf::Rect<T>& rectA, const sf::Rect<T2>& rectB)
	{
		using result_type = std::common_type_t<T, T2>;
		float halfWidthA = rectA.width / 2.0f;
		float halfHeightA = rectA.height / 2.0f;
		float halfWidthB = rectB.width / 2.0f;
		float halfHeightB = rectB.height / 2.0f;

		sf::Vector2<result_type> centerA(rectA.left + halfWidthA, rectA.top - halfHeightA);
		sf::Vector2<result_type> centerB(rectB.left + halfWidthB, rectB.top + halfHeightB);

		float distanceX = centerA.x - centerB.x;
		float distanceY = centerA.y - centerB.y;
		float minDistanceX = halfWidthA + halfWidthB;
		float minDistanceY = halfHeightA + halfHeightB;

		if (abs(distanceX) >= minDistanceX || abs(distanceY) >= minDistanceY)
			return sf::Vector2<result_type>();

		float depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
		float depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;

		if (abs(depthX) > abs(depthY))
			depthX = 0;
		else
			depthY = 0;

		return sf::Vector2<result_type>(depthX, depthY);
	}
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
#pragma once
#ifndef MATH
#define MATH
#include <iostream>
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
namespace Engine 
{
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

		Vector2D& operator= (const Vector2D& v2);
		Vector2D& operator+= (const Vector2D& v2);
		Vector2D& operator-= (const Vector2D& v2);
		Vector2D& operator*= (const float scalar);
		Vector2D& operator/= (const float scalar);

		const Vector2D operator+(const Vector2D &v2) const;
		const Vector2D operator-(const Vector2D &v2) const;
		const Vector2D operator*(const float scalar) const;
		const Vector2D operator/(const float scalar) const;

		bool operator== (const Vector2D& v2) const;
		bool operator> (const Vector2D& v2) const;
		bool operator< (const Vector2D& v2) const;
		bool operator>= (const Vector2D& v2) const;
		bool operator<= (const Vector2D& v2) const;
		bool operator!= (const Vector2D& v2) const;

		friend std::ostream& operator<<(std::ostream& os, const Vector2D& dt);

	public:
		float x, y;
	};
    //     rect1(left,top) |------------------------| rect2(left,top)
    //                     |                        |
    //                     |                        |
    //                     |                        |
    //                     |                        |
    // rect2(widht,height) |------------------------| rect1(widgt,heigt)
	class Quad
	{
	public:
		Quad(float left, float top, float left2, float top2, float widht, float height, float widht2, float height2) : rect1(left, top, widht, height), rect2(left2, top2, widht2, height2) {}
		Quad() = default;
		sf::FloatRect rect1;
		sf::FloatRect rect2;
	};
	class Rectangle
	{
	public:
		Rectangle(double x = 0, double y = 0, double w = 0, double h = 0);
		~Rectangle(void) {};

		inline double Left(void) const { return x; }
		inline double Right(void) const { return x + w; }
		inline double Top(void) const { return y; }
		inline double Bottom(void) const { return y + h; }

		bool Contains(Vector2D& vVec) const;
		bool Contains(double x, double y) const;

		sf::IntRect getSfmlRect_i();
		sf::FloatRect *getSfmlRect_f();
		static Rectangle fromSfmlRect(sf::FloatRect);

		static Rectangle Empty();

		Rectangle getIntersectionRect(const Rectangle& rect);
		static Vector2D GetIntersectionDepth(const Rectangle& rectA, const Rectangle& rectB);
		static Vector2D GetBottomCenter(const Rectangle& rect);
		static Vector2D GetCenter(const Rectangle& rect);
		static float GetDistance(const Rectangle& rectA, const Rectangle& rectB);
		static Vector2D GetDirection(const Rectangle& rectA, const Rectangle& rectB);

		Rectangle& operator= (const Rectangle& r2);

		bool operator== (const Rectangle& r2) const;
		bool operator!= (const Rectangle& r2) const;

	public:
		float x, y, w, h;
	};
}
#endif
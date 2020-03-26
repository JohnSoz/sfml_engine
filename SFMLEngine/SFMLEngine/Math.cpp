﻿#include "Math.h"
#include <map>
using namespace Engine;

//-----------------------------------------------------------------------------
// Задача:	Конструктор
//-----------------------------------------------------------------------------
Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

//-----------------------------------------------------------------------------
// Задача:	Поворачивает вектор
//-----------------------------------------------------------------------------
void Vector2D::Rotate(const float angle)
{
	float xt = (x * cosf(angle)) - (y * sinf(angle));
	float yt = (y * cosf(angle)) + (x * sinf(angle));
	x = xt;
	y = yt;
}

//-----------------------------------------------------------------------------
// Задача: Magnitude
//-----------------------------------------------------------------------------
float Vector2D::Magnitude() const
{
	return sqrtf(x * x + y * y);
}

//-----------------------------------------------------------------------------
// Задача:	Нормализует вектор 
//-----------------------------------------------------------------------------
float Vector2D::Normalize()
{
	float mag = Magnitude();

	if (mag != 0.0)
	{
		x /= mag;
		y /= mag;
	}

	return mag;
}

//-----------------------------------------------------------------------------
// Задача:	Скалярное произведение
//-----------------------------------------------------------------------------
float Vector2D::DotProduct(const Vector2D& v2) const
{
	return (x * v2.x) + (y * v2.y);
}

//-----------------------------------------------------------------------------
// Задача:	Перекрестное произведение
//-----------------------------------------------------------------------------
float Vector2D::CrossProduct(const Vector2D& v2) const
{
	return (x * v2.y) - (y * v2.x);
}

//-----------------------------------------------------------------------------
// Задача:	Пустой вектор
//-----------------------------------------------------------------------------
Vector2D Vector2D::Zero()
{
	return Vector2D(0, 0);
}

//-----------------------------------------------------------------------------
// Задача:	Дистанция между двумя векторами
//-----------------------------------------------------------------------------
float Vector2D::Distance(const Vector2D& v1, const Vector2D& v2)
{
	return sqrtf(pow((v2.x - v1.x), 2) + pow((v2.y - v1.y), 2));
}

sf::Vector2f Vector2D::GetSfmlVector()
{
	return sf::Vector2f(x, y);
}

Vector2D Vector2D::FromSfmlVec(sf::Vector2f vec)
{
	return Vector2D(vec.x, vec.y);
}

Vector2D& Vector2D::operator=(const Vector2D& v2)
{
	if (this == &v2)
		return *this;

	x = v2.x;
	y = v2.y;

	return *this;
}

Vector2D& Vector2D::operator+=(const Vector2D& v2)
{
	x += v2.x;
	y += v2.y;

	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& v2)
{
	x -= v2.x;
	y -= v2.y;

	return *this;
}

Vector2D& Vector2D::operator*=(const float scalar)
{
	x *= scalar;
	y *= scalar;

	return *this;
}

Vector2D& Vector2D::operator/=(const float scalar)
{
	x /= scalar;
	y /= scalar;

	return *this;
}

const Vector2D Vector2D::operator+(const Vector2D& v2) const
{
	return Vector2D(*this) += v2;
}

const Vector2D Vector2D::operator-(const Vector2D& v2) const
{
	return Vector2D(*this) -= v2;
}

const Vector2D Vector2D::operator*(const float scalar) const
{
	return Vector2D(*this) *= scalar;
}

const Vector2D Vector2D::operator/(const float scalar) const
{
	return Vector2D(*this) /= scalar;
}

bool Vector2D::operator==(const Vector2D& v2) const
{
	return ((x == v2.x) && (y == v2.y));
}

bool Vector2D::operator>(const Vector2D& v2) const
{
	return ((x > v2.x) && (y > v2.y));
}

bool Vector2D::operator<(const Vector2D& v2) const
{
	return !(*this > v2);
}

bool Vector2D::operator>=(const Vector2D& v2) const
{
	return !((*this < v2) && (*this == v2));
}

bool Vector2D::operator<=(const Vector2D& v2) const
{
	return ((*this < v2) && (*this == v2));
}

bool Vector2D::operator!=(const Vector2D& v2) const
{
	return !((x == v2.x) && (y == v2.y));
}

#include <stdlib.h>

//-----------------------------------------------------------------------------
// Задача: Конструктор
//-----------------------------------------------------------------------------
Rectangle::Rectangle(double x, double y, double w, double h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

Rectangle::~Rectangle()
{
}

float Rectangle::Left() const
{
	return x;
}

float Rectangle::Right() const
{
	return x + w;
}

float Rectangle::Top() const
{
	return y;
}

float Rectangle::Bottom() const
{
	return y + h;
}

//-----------------------------------------------------------------------------
// Задача: Проверяет содержит ли прямоугольник вектор
//-----------------------------------------------------------------------------
bool Rectangle::Contains(Vector2D& vVec) const
{
	if ((vVec.x >= x) &&
		(vVec.x <= x + w) &&
		(vVec.y >= y) &&
		(vVec.x <= y + h))
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Задача: Содержит ли прямоугольник координаты x и y
//-----------------------------------------------------------------------------
bool Rectangle::Contains(double x, double y) const
{
	if ((x >= this->x) &&
		(x <= (this->x + this->w)) &&
		(y >= this->y) &&
		(x <= (this->y + this->h)))
	{
		return true;
	}
	return false;
}

sf::IntRect Rectangle::getSfmlRect_i()
{
	return sf::IntRect(x, y, w, h);
}

sf::FloatRect* Rectangle::getSfmlRect_f()
{
	return new sf::FloatRect(x, y, w, h);
}

Rectangle Rectangle::fromSfmlRect(sf::FloatRect rect)
{
	return Rectangle(rect.left, rect.top, rect.width, rect.height);
}

//-----------------------------------------------------------------------------
// Задача: Пустой прямоугольник 
//-----------------------------------------------------------------------------
Rectangle Rectangle::Empty()
{
	return Rectangle();
}

Rectangle Rectangle::getIntersectionRect(const Rectangle& rect)
{
	Rectangle intersection = {};
	auto r1MinX = std::min(x, x + w);
	auto r1MaxX = std::max(x, x + w);
	auto r1MinY = std::min(y, y + h);
	auto r1MaxY = std::max(y, y + h);

	// Compute the min and max of the second rectangle on both axes
	auto r2MinX = std::min(rect.x, rect.x + rect.w);
	auto r2MaxX = std::max(rect.x, rect.x + rect.w);
	auto r2MinY = std::min(rect.y, rect.y + rect.h);
	auto r2MaxY = std::max(rect.y, rect.y + rect.h);

	// Compute the intersection boundaries
	auto interLeft = std::max(r1MinX, r2MinX);
	auto interTop = std::max(r1MinY, r2MinY);
	auto interRight = std::min(r1MaxX, r2MaxX);
	auto interBottom = std::min(r1MaxY, r2MaxY);

	// If the intersection is valid (positive non zero area), then there is an intersection
	if ((interLeft < interRight) && (interTop < interBottom))
	{
		intersection = Rectangle(interLeft, interTop, interRight - interLeft, interBottom - interTop);
		return intersection;
	}
	intersection = Rectangle(0, 0, 0, 0);
	return intersection;
}

//-----------------------------------------------------------------------------
// Задача: Глубина пересечения двух прямоугольников
//-----------------------------------------------------------------------------
Vector2D Rectangle::GetIntersectionDepth(const Rectangle& rectA, const Rectangle& rectB)
{
	// Размеры
	float halfWidthA = rectA.w / 2.0f;
	float halfHeightA = rectA.h / 2.0f;
	float halfWidthB = rectB.w / 2.0f;
	float halfHeightB = rectB.h / 2.0f;

	// Центр
	Vector2D centerA(rectA.x + halfWidthA, rectA.y + halfHeightA);
	Vector2D centerB(rectB.x + halfWidthB, rectB.y + halfHeightB);

	// Вычислить текущие и минимальные непересекающиеся расстояния между центрами
	float distanceX = centerA.x - centerB.x;
	float distanceY = centerA.y - centerB.y;
	float minDistanceX = halfWidthA + halfWidthB;
	float minDistanceY = halfHeightA + halfHeightB;

	// Если мы вообще не пересекаемся,  (0, 0)
	if (abs(distanceX) >= minDistanceX || abs(distanceY) >= minDistanceY)
		return Vector2D::Zero();

	// Вычисление и возврат глубины пересечения
	float depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
	float depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;
	if (abs(depthX) > abs(depthY))
		depthX = 0;
	else
		depthY = 0;
	return Vector2D(depthX, depthY);
}

Vector2D Engine::Rectangle::GetIntersectionDepth2(const Rectangle& rectA, const Rectangle& rectB) //< что-то вроде костыля Нужно юзать когда origin спрайта = 
																									  // sprite.pos.x + sprite.rect.width/2, sprite.pos.y + sprite.rect.height
{
	float halfWidthA = rectA.w / 2.0f;
	float halfHeightA = rectA.h / 2.0f;
	float halfWidthB = rectB.w / 2.0f;
	float halfHeightB = rectB.h / 2.0f;

	Vector2D centerA(rectA.x + halfWidthA, rectA.y - halfHeightA);
	Vector2D centerB(rectB.x + halfWidthB, rectB.y + halfHeightB);

	float distanceX = centerA.x - centerB.x;
	float distanceY = centerA.y - centerB.y;
	float minDistanceX = halfWidthA + halfWidthB;
	float minDistanceY = halfHeightA + halfHeightB;

	if (abs(distanceX) >= minDistanceX || abs(distanceY) >= minDistanceY)
		return Vector2D::Zero();

	float depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
	float depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;

	if (abs(depthX) > abs(depthY))
		depthX = 0;
	else
		depthY = 0;

	return Vector2D(depthX, depthY);
}

//-----------------------------------------------------------------------------
// Задача: Возвращает положение центра нижнего края прямоугольника.
//-----------------------------------------------------------------------------
Vector2D Rectangle::GetBottomCenter(const Rectangle& rect)
{
	return Vector2D((float)(rect.x + rect.w / 2.0f), (float)(rect.y + rect.h));
}

//-----------------------------------------------------------------------------
// Задача: Определяет положение центральной точки прямоугольника
//-----------------------------------------------------------------------------
Vector2D Rectangle::GetCenter(const Rectangle& rect)
{
	return Vector2D((float)(rect.x + rect.w / 2.0f), (float)(rect.y + rect.h / 2.0f));
}

//-----------------------------------------------------------------------------
// Задача: Возвращает расстояние с плавающей точкой между центральной точкой 
// одного прямоугольника и Центральной точки другого.
//-----------------------------------------------------------------------------
float Rectangle::GetDistance(const Rectangle& rectA, const Rectangle& rectB)
{
	return Vector2D::Distance(GetCenter(rectA), GetCenter(rectB));
}

//-----------------------------------------------------------------------------
// Задача: Возвращает единичный вектор от одного прямоугольника к другому
//-----------------------------------------------------------------------------
Vector2D Rectangle::GetDirection(const Rectangle& rectA, const Rectangle& rectB)
{
	Vector2D direction = GetCenter(rectA) - GetCenter(rectB);
	direction.Normalize();
	return direction;
}

Rectangle& Rectangle::operator=(const Rectangle& r2)
{
	if (this == &r2)
		return *this;

	x = r2.x;
	y = r2.y;
	w = r2.w;
	h = r2.h;

	return *this;
}

bool Rectangle::operator==(const Rectangle& r2) const
{
	return ((w == r2.w) && (h == r2.h));
}

bool Rectangle::operator!=(const Rectangle& r2) const
{
	return !((w == r2.w) && (h == r2.h));
}

std::ostream& Engine::operator<<(std::ostream& os, const Vector2D& dt)
{
	os << "  X = " << dt.x << " Y = " << dt.y;
	return os;
}

sf::Vector2f Engine::VectorAbs(sf::Vector2f vec)
{
	sf::Vector2f absVec;
	absVec.x = std::abs(vec.x);
	absVec.y = std::abs(vec.x);
	return absVec;
}


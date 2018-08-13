#include "Math.h"
using namespace Engine;
//-----------------------------------------------------------------------------
// ������:	�����������
//-----------------------------------------------------------------------------
Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

//-----------------------------------------------------------------------------
// ������:	������������ ������
//-----------------------------------------------------------------------------
void Vector2D::Rotate(const float angle)
{
	float xt = (x * cosf(angle)) - (y * sinf(angle));
	float yt = (y * cosf(angle)) + (x * sinf(angle));
	x = xt;
	y = yt;
}

//-----------------------------------------------------------------------------
// ������: Magnitude
//-----------------------------------------------------------------------------
float Vector2D::Magnitude() const
{
	return sqrtf(x * x + y * y);
}

//-----------------------------------------------------------------------------
// ������:	����������� ������ 
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
// ������:	��������� ������������
//-----------------------------------------------------------------------------
float Vector2D::DotProduct(const Vector2D &v2) const
{
	return (x * v2.x) + (y * v2.y);
}

//-----------------------------------------------------------------------------
// ������:	������������ ������������
//-----------------------------------------------------------------------------
float Vector2D::CrossProduct(const Vector2D &v2) const
{
	return (x * v2.y) - (y * v2.x);
}

//-----------------------------------------------------------------------------
// ������:	������ ������
//-----------------------------------------------------------------------------
Vector2D Vector2D::Zero()
{
	return Vector2D(0, 0);
}

//-----------------------------------------------------------------------------
// ������:	��������� ����� ����� ���������
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

Vector2D& Vector2D::operator= (const Vector2D& v2)
{
	if (this == &v2)
		return *this;

	x = v2.x;
	y = v2.y;

	return *this;
}

Vector2D& Vector2D::operator+= (const Vector2D& v2)
{
	x += v2.x;
	y += v2.y;

	return *this;
}

Vector2D& Vector2D::operator-= (const Vector2D& v2)
{
	x -= v2.x;
	y -= v2.y;

	return *this;
}

Vector2D& Vector2D::operator*= (const float scalar)
{
	x *= scalar;
	y *= scalar;

	return *this;
}

Vector2D& Vector2D::operator/= (const float scalar)
{
	x /= scalar;
	y /= scalar;

	return *this;
}

const Vector2D Vector2D::operator+(const Vector2D &v2) const
{
	return Vector2D(*this) += v2;
}

const Vector2D Vector2D::operator-(const Vector2D &v2) const
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

bool Vector2D::operator== (const Vector2D& v2) const
{
	return ((x == v2.x) && (y == v2.y));
}

bool Vector2D::operator>(const Vector2D & v2) const
{
	return ((x > v2.x) && (y > v2.y));
}

bool Vector2D::operator<(const Vector2D & v2) const
{
	return !(*this > v2);
}

bool Vector2D::operator>=(const Vector2D & v2) const
{
	return !((*this < v2) && (*this == v2));
}

bool Vector2D::operator<=(const Vector2D & v2) const
{
	return ((*this < v2) && (*this == v2));
}

bool Vector2D::operator!= (const Vector2D& v2) const
{
	return !((x == v2.x) && (y == v2.y));
}

#include <stdlib.h>

//-----------------------------------------------------------------------------
// ������: �����������
//-----------------------------------------------------------------------------
Rectangle::Rectangle(int x, int y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

//-----------------------------------------------------------------------------
// ������: ��������� �������� �� ������������� ������
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
	else
		return false;
}

//-----------------------------------------------------------------------------
// ������: �������� �� ������������� ���������� x � y
//-----------------------------------------------------------------------------
bool Rectangle::Contains(int x, int y) const
{
	if ((x >= this->x) &&
		(x <= this->x + this->w) &&
		(y >= this->y) &&
		(x <= this->y + this->h))
	{
		return true;
	}
	else
		return false;
}

sf::IntRect Rectangle::getSfmlRect()
{
	return sf::IntRect(x, y, w, h);
}

Rectangle Rectangle::fromSfmlRect(sf::IntRect rect)
{
	return Rectangle(rect.left, rect.top, rect.width, rect.height);
}

//-----------------------------------------------------------------------------
// ������: ������ ������������� 
//-----------------------------------------------------------------------------
Rectangle Rectangle::Empty()
{
	return Rectangle();
}

//-----------------------------------------------------------------------------
// ������: ������� ����������� ���� ���������������
//-----------------------------------------------------------------------------
Vector2D Rectangle::GetIntersectionDepth(const Rectangle& rectA, const Rectangle& rectB)
{
	// �������.
	float halfWidthA = rectA.w / 2.0f;
	float halfHeightA = rectA.h / 2.0f;
	float halfWidthB = rectB.w / 2.0f;
	float halfHeightB = rectB.h / 2.0f;

	// �����.
	Vector2D centerA(rectA.x + halfWidthA, rectA.y + halfHeightA);
	Vector2D centerB(rectB.x + halfWidthB, rectB.y + halfHeightB);

	// ��������� ������� � ����������� ���������������� ���������� ����� ��������.
	float distanceX = centerA.x - centerB.x;
	float distanceY = centerA.y - centerB.y;
	float minDistanceX = halfWidthA + halfWidthB;
	float minDistanceY = halfHeightA + halfHeightB;

	// ���� �� ������ �� ������������,  (0, 0).
	if (abs(distanceX) >= minDistanceX || abs(distanceY) >= minDistanceY)
		return Vector2D::Zero();

	// ���������� � ������� ������� �����������.
	float depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
	float depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;
	if (abs(depthX) > abs(depthY))
		depthX = 0;
	else
		depthY = 0;
	return Vector2D(depthX, depthY);
}

//-----------------------------------------------------------------------------
// ������: ���������� ��������� ������ ������� ���� ��������������.
//-----------------------------------------------------------------------------
Vector2D Rectangle::GetBottomCenter(const Rectangle& rect)
{
	return Vector2D((float)(rect.x + rect.w / 2.0f), (float)(rect.y + rect.h));
}

//-----------------------------------------------------------------------------
// ������: ���������� ��������� ����������� ����� ��������������
//-----------------------------------------------------------------------------
Vector2D Rectangle::GetCenter(const Rectangle& rect)
{
	return Vector2D((float)(rect.x + rect.w / 2.0f), (float)(rect.y + rect.h / 2.0f));
}

//-----------------------------------------------------------------------------
// ������: ���������� ���������� � ��������� ������ ����� ����������� ������ 
// ������ �������������� � ����������� ����� �������.
//-----------------------------------------------------------------------------
float Rectangle::GetDistance(const Rectangle& rectA, const Rectangle& rectB)
{
	return Vector2D::Distance(GetCenter(rectA), GetCenter(rectB));
}

//-----------------------------------------------------------------------------
// ������: ���������� ��������� ������ �� ������ �������������� � �������
//-----------------------------------------------------------------------------
Vector2D Rectangle::GetDirection(const Rectangle& rectA, const Rectangle& rectB)
{
	Vector2D direction = GetCenter(rectA) - GetCenter(rectB);
	direction.Normalize();
	return direction;
}

Rectangle& Rectangle::operator= (const Rectangle& r2)
{
	if (this == &r2)
		return *this;

	x = r2.x;
	y = r2.y;
	w = r2.w;
	h = r2.h;

	return *this;
}

bool Rectangle::operator== (const Rectangle& r2) const
{
	return ((w == r2.w) && (h == r2.h));
}

bool Rectangle::operator!= (const Rectangle& r2) const
{
	return !((w == r2.w) && (h == r2.h));
}

std::ostream & Engine::operator<<(std::ostream & os, const Vector2D & dt)
{
	os << "  X = " << dt.x << " Y = " << dt.y;
	return os;
}

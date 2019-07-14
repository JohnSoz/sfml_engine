#include "Math.h"
#include <map>
using namespace Engine;

namespace Collision
{
	void SAT::normalize(sf::Vector2f& vector)
	{
		const float length = sqrt(vector.x * vector.x + vector.y * vector.y);
		if (length == 0)
			return;
		vector.x = vector.x / length;
		vector.y = vector.y / length;
	}

	float SAT::dot(sf::Vector2f& vector1, sf::Vector2f& vector2)
	{
		return vector1.x * vector2.x + vector1.y * vector2.y;
	}

	float SAT::distance(float minA, float maxA, float minB, float maxB) // Positive return = no hit
	{
		if (minB > maxA)
			return minB - maxA;
		return minA - maxB;
	}


	void SAT::project(sf::Vector2f& axis, rectangle* _rectangle, float& min, float& max)
	{
		float _dot = dot(axis, _rectangle->vertex[0]);

		min = _dot;
		max = _dot;

		for (short i = 1; i < 4; i++)
		{
			_dot = dot(_rectangle->vertex[i], axis);

			if (_dot < min)
				min = _dot;
			else if (_dot > max)
				max = _dot;
		}
	}


	bool SAT::collision(sf::Sprite* s1, sf::Sprite* s2)
	{
		rectangle one;
		rectangle two;

		sf::Transform transform = s1->getTransform();
		sf::FloatRect rect = s1->getLocalBounds();

		one.vertex[0] = transform.transformPoint(sf::Vector2f(rect.left, rect.top));
		one.vertex[1] = transform.transformPoint(sf::Vector2f(rect.left + rect.width, rect.top));
		one.vertex[2] = transform.transformPoint(sf::Vector2f(rect.left + rect.width, rect.top + rect.height));
		one.vertex[3] = transform.transformPoint(sf::Vector2f(rect.left, rect.top + rect.height));

		transform = s2->getTransform();
		rect = s2->getLocalBounds();

		two.vertex[0] = transform.transformPoint(sf::Vector2f(rect.left, rect.top));
		two.vertex[1] = transform.transformPoint(sf::Vector2f(rect.left + rect.width, rect.top));
		two.vertex[2] = transform.transformPoint(sf::Vector2f(rect.left + rect.width, rect.top + rect.height));
		two.vertex[3] = transform.transformPoint(sf::Vector2f(rect.left, rect.top + rect.height));

		sf::Vector2f axis;
		float minA;
		float minB;
		float maxA;
		float maxB;


		// Test one
		axis.x = one.vertex[1].x - one.vertex[0].x;
		axis.y = one.vertex[1].y - one.vertex[0].y;
		normalize(axis);

		project(axis, &one, minA, maxA);
		project(axis, &two, minB, maxB);

		if (distance(minA, maxA, minB, maxB) > 0.f)
			return false;


		// Test two
		axis.x = one.vertex[3].x - one.vertex[0].x;
		axis.y = one.vertex[3].y - one.vertex[0].y;
		normalize(axis);

		project(axis, &one, minA, maxA);
		project(axis, &two, minB, maxB);

		if (distance(minA, maxA, minB, maxB) > 0.f)
			return false;


		// Test three
		axis.x = two.vertex[1].x - two.vertex[0].x;
		axis.y = two.vertex[1].y - two.vertex[0].y;
		normalize(axis);

		project(axis, &one, minA, maxA);
		project(axis, &two, minB, maxB);

		if (distance(minA, maxA, minB, maxB) > 0.f)
			return false;


		// Test four
		axis.x = two.vertex[3].x - two.vertex[0].x;
		axis.y = two.vertex[3].y - two.vertex[0].y;
		normalize(axis);

		project(axis, &one, minA, maxA);
		project(axis, &two, minB, maxB);

		if (distance(minA, maxA, minB, maxB) > 0.f)
			return false;

		return true;
	}
}

namespace Collision
{
	class BitmaskManager
	{
	public:
		~BitmaskManager()
		{
			std::map<const sf::Texture*, sf::Uint8*>::const_iterator end = Bitmasks.end();
			for (std::map<const sf::Texture*, sf::Uint8*>::const_iterator iter = Bitmasks.begin(); iter != end; ++iter)
				delete[] iter->second;
		}

		sf::Uint8 GetPixel(const sf::Uint8* mask, const sf::Texture* tex, unsigned int x, unsigned int y)
		{
			if (x > tex->getSize().x || y > tex->getSize().y)
				return 0;

			return mask[x + y * tex->getSize().x];
		}

		sf::Uint8* GetMask(const sf::Texture* tex)
		{
			sf::Uint8* mask;
			auto pair = Bitmasks.find(tex);
			if (pair == Bitmasks.end())
			{
				sf::Image img = tex->copyToImage();
				mask = CreateMask(tex, img);
			}
			else
				mask = pair->second;

			return mask;
		}

		sf::Uint8* CreateMask(const sf::Texture* tex, const sf::Image& img)
		{
			sf::Uint8* mask = new sf::Uint8[tex->getSize().y * tex->getSize().x];

			for (unsigned int y = 0; y < tex->getSize().y; y++)
			{
				for (unsigned int x = 0; x < tex->getSize().x; x++)
				{
					size_t index = x + y * tex->getSize().x;
					mask[index] = img.getPixel(x, y).a;
				}
			}

			Bitmasks.insert(std::pair<const sf::Texture*, sf::Uint8*>(tex, mask));

			return mask;
		}

	private:
		std::map<const sf::Texture*, sf::Uint8*> Bitmasks;
	};

	BitmaskManager Bitmasks;

	bool PixelPerfectTest(const sf::Sprite& Object1, const sf::Sprite& Object2, sf::Uint8 AlphaLimit)
	{
		sf::FloatRect Intersection;
		if (Object1.getGlobalBounds().intersects(Object2.getGlobalBounds(), Intersection))
		{
			const auto O1SubRect = Object1.getTextureRect();
			const auto O2SubRect = Object2.getTextureRect();

			auto mask1 = Bitmasks.GetMask(Object1.getTexture());
			auto mask2 = Bitmasks.GetMask(Object2.getTexture());

			// Loop through our pixels
			for (auto i = Intersection.left; i < Intersection.left + Intersection.width; i++)
			{
				for (auto j = Intersection.top; j < Intersection.top + Intersection.height; j++)
				{
					const auto o1v = Object1.getInverseTransform().transformPoint(i, j);
					const auto o2v = Object2.getInverseTransform().transformPoint(i, j);

					// Make sure pixels fall within the sprite's subrect
					if (o1v.x > 0 && o1v.y > 0 && o2v.x > 0 && o2v.y > 0 &&
						o1v.x < O1SubRect.width && o1v.y < O1SubRect.height &&
						o2v.x < O2SubRect.width && o2v.y < O2SubRect.height)
					{
						if (Bitmasks.GetPixel(mask1, Object1.getTexture(), static_cast<int>(o1v.x) + O1SubRect.left,
							static_cast<int>(o1v.y) + O1SubRect.top) > AlphaLimit &&
							Bitmasks.GetPixel(mask2, Object2.getTexture(), static_cast<int>(o2v.x) + O2SubRect.left,
								static_cast<int>(o2v.y) + O2SubRect.top) > AlphaLimit)
							return true;
					}
				}
			}
		}
		return false;
	}

	bool CreateTextureAndBitmask(sf::Texture& LoadInto, const std::string& Filename)
	{
		sf::Image img;
		if (!img.loadFromFile(Filename))
			return false;
		if (!LoadInto.loadFromImage(img))
			return false;

		Bitmasks.CreateMask(&LoadInto, img);
		return true;
	}

	sf::Vector2f GetSpriteCenter(const sf::Sprite& Object)
	{
		sf::FloatRect AABB = Object.getGlobalBounds();
		return sf::Vector2f(AABB.left + AABB.width / 2.f, AABB.top + AABB.height / 2.f);
	}

	sf::Vector2f GetSpriteSize(const sf::Sprite& Object)
	{
		sf::IntRect OriginalSize = Object.getTextureRect();
		sf::Vector2f Scale = Object.getScale();
		return sf::Vector2f(OriginalSize.width * Scale.x, OriginalSize.height * Scale.y);
	}

	bool CircleTest(const sf::Sprite& Object1, const sf::Sprite& Object2)
	{
		sf::Vector2f Obj1Size = GetSpriteSize(Object1);
		sf::Vector2f Obj2Size = GetSpriteSize(Object2);
		float Radius1 = (Obj1Size.x + Obj1Size.y) / 4;
		float Radius2 = (Obj2Size.x + Obj2Size.y) / 4;

		sf::Vector2f Distance = GetSpriteCenter(Object1) - GetSpriteCenter(Object2);

		return (Distance.x * Distance.x + Distance.y * Distance.y <= (Radius1 + Radius2) * (Radius1 + Radius2));
	}

	class OrientedBoundingBox // Used in the BoundingBoxTest
	{
	public:
		OrientedBoundingBox(const sf::Sprite& Object)
			// Calculate the four points of the OBB from a transformed (scaled, rotated...) sprite
		{
			sf::Transform trans = Object.getTransform();
			sf::IntRect local = Object.getTextureRect();
			Points[0] = trans.transformPoint(0.f, 0.f);
			Points[1] = trans.transformPoint(local.width, 0.f);
			Points[2] = trans.transformPoint(local.width, local.height);
			Points[3] = trans.transformPoint(0.f, local.height);
		}

		sf::Vector2f Points[4];

		void ProjectOntoAxis(const sf::Vector2f& Axis, float& Min, float& Max)
			// Project all four points of the OBB onto the given axis and return the dotproducts of the two outermost points
		{
			Min = (Points[0].x * Axis.x + Points[0].y * Axis.y);
			Max = Min;
			for (int j = 1; j < 4; j++)
			{
				float Projection = (Points[j].x * Axis.x + Points[j].y * Axis.y);

				if (Projection < Min)
					Min = Projection;
				if (Projection > Max)
					Max = Projection;
			}
		}
	};

	bool BoundingBoxTest(const sf::Sprite& Object1, const sf::Sprite& Object2)
	{
		OrientedBoundingBox OBB1(Object1);
		OrientedBoundingBox OBB2(Object2);

		// Create the four distinct axes that are perpendicular to the edges of the two rectangles
		sf::Vector2f Axes[4] = {
			sf::Vector2f(OBB1.Points[1].x - OBB1.Points[0].x,
						 OBB1.Points[1].y - OBB1.Points[0].y),
			sf::Vector2f(OBB1.Points[1].x - OBB1.Points[2].x,
						 OBB1.Points[1].y - OBB1.Points[2].y),
			sf::Vector2f(OBB2.Points[0].x - OBB2.Points[3].x,
						 OBB2.Points[0].y - OBB2.Points[3].y),
			sf::Vector2f(OBB2.Points[0].x - OBB2.Points[1].x,
						 OBB2.Points[0].y - OBB2.Points[1].y)
		};

		for (int i = 0; i < 4; i++) // For each axis...
		{
			float MinOBB1, MaxOBB1, MinOBB2, MaxOBB2;

			// ... project the points of both OBBs onto the axis ...
			OBB1.ProjectOntoAxis(Axes[i], MinOBB1, MaxOBB1);
			OBB2.ProjectOntoAxis(Axes[i], MinOBB2, MaxOBB2);

			// ... and check whether the outermost projected points of both OBBs overlap.
			// If this is not the case, the Separating Axis Theorem states that there can be no collision between the rectangles
			if (!((MinOBB2 <= MaxOBB1) && (MaxOBB2 >= MinOBB1)))
				return false;
		}
		return true;
	}
}

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
	// Размеры.
	float halfWidthA = rectA.w / 2.0f;
	float halfHeightA = rectA.h / 2.0f;
	float halfWidthB = rectB.w / 2.0f;
	float halfHeightB = rectB.h / 2.0f;

	// Центр.
	Vector2D centerA(rectA.x + halfWidthA, rectA.y + halfHeightA);
	Vector2D centerB(rectB.x + halfWidthB, rectB.y + halfHeightB);

	// Вычислить текущие и минимальные непересекающиеся расстояния между центрами.
	float distanceX = centerA.x - centerB.x;
	float distanceY = centerA.y - centerB.y;
	float minDistanceX = halfWidthA + halfWidthB;
	float minDistanceY = halfHeightA + halfHeightB;

	// Если мы вообще не пересекаемся,  (0, 0).
	if (abs(distanceX) >= minDistanceX || abs(distanceY) >= minDistanceY)
		return Vector2D::Zero();

	// Вычисление и возврат глубины пересечения.
	float depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
	float depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;

	return Vector2D(depthX, depthY);
}

Vector2D Engine::Rectangle::GetIntersectionDepth2(const Rectangle& rectA, const Rectangle& rectB)
{
	// Размеры.
	float halfWidthA = rectA.w / 2.0f;
	float halfHeightA = rectA.h;// / 2.0f;
	float halfHeightA2 = rectA.h / 2.0f;
	float halfWidthB = rectB.w / 2.0f;
	float halfHeightB = rectB.h / 2.0f;

	// Центр.
	Vector2D centerA(rectA.x + halfWidthA, rectA.y);
	Vector2D centerA2(rectA.x + halfWidthA, rectA.y + halfHeightA2);
	Vector2D centerB(rectB.x + halfWidthB, rectB.y + halfHeightB);

	// Вычислить текущие и минимальные непересекающиеся расстояния между центрами.
	float distanceX = centerA.x - centerB.x;
	float distanceY = centerA.y - centerB.y;
	float distanceY2 = centerA2.y - centerB.y;
	float minDistanceX = halfWidthA + halfWidthB;
	float minDistanceY = std::max(halfHeightB, halfHeightA);
	float minDistanceY2 = halfHeightA2 + halfHeightB;

	if (abs(distanceX) >= minDistanceX || abs(distanceY) >= minDistanceY)
		return Vector2D::Zero();

	// Вычисление и возврат глубины пересечения.
	float depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
	float depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;
	float depthY2 = distanceY2 > 0 ? minDistanceY2 - distanceY2 : -minDistanceY2 - distanceY2;
	if (depthX < depthY2)
		depthY = 0;
	if (abs(depthX) > abs(depthY2))
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


#include "Actor.h"
#include "Math.h"
using sf::Keyboard;

void Engine::Actor::RotateToMouse(float speed, sf::RenderWindow& window)
{
	//LastAngle = sprite.getRotation();

	//sf::Vector2f posMouse = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
	//float a = posMouse.x - sprite.getPosition().x;
	//float v = posMouse.y - sprite.getPosition().y;

	//Radian = atan2f(v, a);
	//CurrAngle = (atan2f(v, a)) * 180 / PI;
	//if (CurrAngle > 180) CurrAngle -= 360;
	//if (CurrAngle < -180) CurrAngle += 360;
	//posMouse.x = CurrAngle - LastAngle;
	//posMouse.y = 360 + posMouse.x;
	//if (posMouse.y > 360) posMouse.y -= 720;
	//if (abs(posMouse.x) > abs(posMouse.y)) posMouse.x = posMouse.y;

	//if (CurrAngle != LastAngle)
	//{
	//	if (abs(posMouse.x) < speed)
	//	{
	//		LastAngle = CurrAngle;
	//	}
	//	else
	//	{
	//		if (posMouse.x > 0)
	//			LastAngle += speed;
	//		else
	//			LastAngle -= speed;
	//	}
	//}

	//float lastradian = LastAngle * PI / 180;
	//if (lastradian <= 1 && lastradian >= -2.5)
	//	PointOfFire.setPosition(sf::Vector2f(position.x + cosf(lastradian - 0.1) * 46, position.y + sinf(lastradian - 0.1) * 46));
	//else
	//	PointOfFire.setPosition(sf::Vector2f(position.x + cosf(lastradian - 0.05) * 46, position.y + sinf(lastradian - 0.05) * 46));

	////////////////////////////////////////////////rotate debug rectangle////////////////////////
	//auto x = globalRectangle.left;
	//auto y = globalRectangle.top;
	//auto w = globalRectangle.width;
	//auto h = globalRectangle.height;

	//float centerX = position.x;
	//float centerY = position.y;

	//auto newX = centerX + (x - centerX) * cos(lastradian) - (y - centerY) * sin(lastradian); //x,y
	//auto newY = centerY + (y - centerY) * cos(lastradian) + (x - centerX) * sin(lastradian); //y,x
	//auto newW = centerX + (w - centerX) * cos(lastradian) - (h - centerY) * sin(lastradian); //w,h
	//auto newH = centerY + (h - centerY) * cos(lastradian) + (w - centerX) * sin(lastradian); //h,w

	//auto newX2 = centerX + (x - centerX) * cos(lastradian) - (h - centerY) * sin(lastradian); //x,h
	//auto newY2 = centerY + (h - centerY) * cos(lastradian) + (x - centerX) * sin(lastradian); //h,x
	//auto newW2 = centerX + (w - centerX) * cos(lastradian) - (y - centerY) * sin(lastradian); //w,y
	//auto newY3 = centerY + (y - centerY) * cos(lastradian) + (w - centerX) * sin(lastradian); //y,w

	//debugRectangle = sf::FloatRect(newX2, newY2, newW2, newY3);
	//globalRectangle = sf::FloatRect(newX, newY, newW, newH);

	//if (window.hasFocus())
	//	sprite.setRotation(LastAngle);
}

void Engine::Actor::updateSprite()
{
	auto currAnim = animManager.GetCurrAnimation<AnimationXml>();
	localRectangle = currAnim->rect;
	originOffset = currAnim->origin;
	scale = currAnim->scale;
	//sprite.setTexture(currAnim->texture);
	switch (direction)
	{
	case DirectionX::Left:
		sprite.setScale(scale * -1, scale);
		break;
	case DirectionX::Right:
		sprite.setScale(scale, scale);
		break;
	}
	sprite.setOrigin(originOffset);
	sprite.setTextureRect(localRectangle);
	sprite.setPosition(position);
}

Engine::Actor::Actor(sf::Vector2f POSITION, std::string NAME, sf::RenderWindow& w, Level& lvl, std::string_view animation) : Entity(POSITION, NAME)
{
	type = OActor;
	direction = Left;
	//globalRectangle = sf::FloatRect(position.x, position.y, position.x + localRectangle.width, position.y + localRectangle.top);
	obj = lvl.GetAllObjects();
	window = &w;
	animManager.LoadAnimation_x(animation);
	auto object_level = *std::find_if(obj.begin(), obj.end(), [](auto Obj) { return (Obj.name == "PlayerSpawn"); });
	position = sf::Vector2f(object_level.rect.left + object_level.rect.width / 2, object_level.rect.top + object_level.rect.height);
	setTexture(animManager.texture);
	updateSprite();
}

void Engine::Actor::handleEvent(sf::Event& e) {}
void Engine::Actor::isKeyPressed() {}


void Engine::Actor::checkClashes(float time) {}

void Engine::Actor::CollisionUpdate(Entity* entity)
{
	auto playerRect = Rectangle::fromSfmlRect(sprite.getGlobalBounds());
	auto objectRect = Rectangle::fromSfmlRect(entity->getRect());
	auto offset = Rectangle::GetIntersectionDepth(playerRect, objectRect);
	position = position + offset.GetSfmlVector();
}

void Engine::Actor::update(float time) {}

void Engine::Actor::start() {}


sf::Vector2f Engine::operator+(const sf::Vector2f& rect, float scale)
{
	return sf::Vector2f(rect.x + scale, rect.y + scale);
}


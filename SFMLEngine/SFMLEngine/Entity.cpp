#include "Entity.h"
#include "LogConsole.h"

Engine::Entity::Entity(sf::Vector2f _position, std::string _name) : Object(_position, _name), collider(_position, sf::Vector2f(0, 0))
{
	setType(ObjectType::OEntity);
}

Engine::Entity::Entity(sf::Image& _image, sf::Vector2f _position, std::string _name) : Object(_image, _position, _name)
{
	setType(ObjectType::OEntity);
	// type = ObjectType::OEntity;
	// texture.loadFromImage(IMAGE);
	// texture.setSmooth(true);
	// sprite.setTexture(texture);
	// sprite.setPosition(position);
}

Engine::Entity::Entity(sf::Image& _image, sf::IntRect _rect, sf::Vector2f _position, std::string _name) : Object(_image, _position, _name), collider(_position, sf::Vector2f(_rect.width, _rect.height))
{
	localRectangle = _rect;
	globalRectangle = sf::FloatRect(position.x, position.y, _rect.width, _rect.height);
	sprite.setTextureRect(_rect);
	setType(ObjectType::OEntity);
}

//void Engine::Test::CollisionUpdate(Entity* entity)
//{
//	if (entity->getName() == "Bullet")
//	{
//		auto b = (Bullet*)entity;
//		if (b->shootersName != this->name)
//		{
//			getDamage();
//			Console::AppLog::addLog("Entity Test2 getDamage, hp = " + std::to_string(hp), Console::info);
//			b->destroy();
//		}
//	}
//}

void Engine::Bullet::CheckClashes()
{
	auto objRect = sf::FloatRect(position.x, position.y, localRectangle.width * scale, localRectangle.height * scale);
	for (auto& o : obj)
	{
		if (objRect.intersects(o.rect))
		{
			destroy();
		}
	}
}

Engine::Bullet::Bullet(sf::Image& _image, sf::IntRect _rect, sf::Vector2f _position, std::string _name, DirectionX _directionX, float _damage, std::string _shootersName)
	: Entity(_image, _rect, _position, _name)
{
	shootersName = _shootersName;
	dir = _directionX;
	damage = _damage;
	sprite.setScale(scale, scale);
	sprite.setOrigin(_rect.width * scale, _rect.height * scale);
}

void Engine::Bullet::CollisionUpdate(Entity*)
{
	std::cout << getName();
}

void Engine::Bullet::update(float time)
{
	if (dir == DirectionX::Left)
		position.x -= speed * time;
	else
		position.x += speed * time;

	float posX = position.x + 1;
	float posY = position.y + 1;
	globalRectangle = sf::FloatRect(posX, posY, posX + localRectangle.width * scale, posY + localRectangle.height * scale);
	debugRectangle = sf::FloatRect(posX + localRectangle.width * scale, posY, posX, posY + localRectangle.height * scale);
	CheckClashes();
	sprite.setPosition(position.x + localRectangle.width * scale / 2, position.y + localRectangle.width * scale / 2);
}

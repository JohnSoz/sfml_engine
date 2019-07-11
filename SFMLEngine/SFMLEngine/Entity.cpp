#include "Entity.h"

Engine::Entity::Entity(sf::Vector2f POSITION, std::string NAME) : Object(POSITION, NAME)
{
	type = ObjectType::OEntity;
}

Engine::Entity::Entity(sf::Image& IMAGE, sf::Vector2f POSITION, std::string NAME) : Object(IMAGE, POSITION, ObjectType::OEntity, NAME)
{
	// type = ObjectType::OEntity;
	// texture.loadFromImage(IMAGE);
	// texture.setSmooth(true);
	// sprite.setTexture(texture);
	// sprite.setPosition(position);
}

Engine::Entity::Entity(sf::Image& i, sf::IntRect r, sf::Vector2f pos, std::string name) : Object(i, pos, ObjectType::OEntity, name)
{
	localRectangle = r;
	globalRectangle = sf::FloatRect(position.x, position.y, r.width, r.height);
	sprite.setTextureRect(r);
}

Engine::Entity::~Entity()
{
}
#include "LogConsole.h"
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
			IsActive = false;
		}
	}
}

Engine::Bullet::Bullet(sf::Image& IMAGE, sf::IntRect r, sf::Vector2f pos, std::string name, DirectionX d, float Damage, std::string nameShooters)
	: Entity(IMAGE, r, pos, name)
{
	shootersName = nameShooters;
	dir = d;
	IsActive = true;
	damage = Damage;
	sprite.setScale(scale, scale);
	sprite.setOrigin(r.width * scale, r.height * scale);
}

void Engine::Bullet::CollisionUpdate(Entity* entity)
{
	std::cout << name;
}

void Engine::Bullet::update(float time)
{
	if (dir == Left)
	{
		position.x -= speed * time;
	}
	else
		position.x += speed * time;

	float posX = position.x + 1;
	float posY = position.y + 1;
	globalRectangle = sf::FloatRect(posX, posY, posX + localRectangle.width * scale, posY + localRectangle.height * scale);
	debugRectangle = sf::FloatRect(posX + localRectangle.width * scale, posY, posX, posY + localRectangle.height * scale);
	CheckClashes();
	sprite.setPosition(position.x + localRectangle.width * scale / 2, position.y + localRectangle.width * scale / 2);
	debug::debugDraw<Bullet, Object, Bullet>(this, name);
}

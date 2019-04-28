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
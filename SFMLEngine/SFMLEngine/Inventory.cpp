#include "Inventory.h"
#include "World.h"
#include "Player.h"
#include "EngineEvents.h"
using namespace Engine;
std::string itemType_s[3] = { "item","gun","heal" };

std::string Engine::Item::getType_s()
{
	return itemType_s[type];
}

void Engine::Gun::action(Player& p)
{
	if (actionClock.getElapsedTime().asMilliseconds() > this->getRate())
	{
		actionClock.restart();
		Image i;
		i.loadFromFile("Data/images/bullet.png");
		auto pos = p.getPos();
		pos.y -= 10;
		pos.x += 5;
		static int id;
		++id;
		Bullet* bullet = new Bullet(i, IntRect(0, 0, 16, 16), pos, "Bullet" + std::to_string(id), p.getDirection(), getDamage(), World::getLevel(), name);
		EventManager::eventManager.emit<Events::NewObject_Event>(*bullet);
	}
}

void Engine::Heal::action(Player& p)
{
	if (actionClock.getElapsedTime().asMilliseconds() > 500)
	{
		p.increaseHealth(20);
		actionClock.restart();
		IsActive = false;
	}
}

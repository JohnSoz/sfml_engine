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

Engine::Gun::Gun(std::string_view Name, float dmg, float rate, float w) :
	RateOfFire(rate), damage(dmg)
{
	AmmoInGun = 0;
	LoadSprite(Name.data());
	weight = w;
	name = Name;
	type = ItemType::gun;
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
		Bullet* bullet = new Bullet(i, IntRect(0, 0, 16, 16), pos, "Bullet" + std::to_string(id), p.getDirection(), getDamage(), name);
		EventManager::eventManager.emit<Events::NewObject_Event>(*bullet);
	}
}

void Engine::Gun::LoadSprite(std::string_view name)

{
	if (name == "pistol")
	{
		texture.loadFromFile("Data/images/Pistolet.png");
		sprite.setTexture(texture);
	}
	else if (name == "rifle")
	{
		texture.loadFromFile("Data/images/AK_47.png");
		sprite.setTexture(texture);
	}
	else if (name == "rocketLauncher")
	{
		texture.loadFromFile("Data/images/RPG.png");
		sprite.setTexture(texture);
	}
	else
	{
		texture.loadFromFile(""); //ERR
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

void Engine::Inventory::baseIni()
{
	inv.push_back(new Gun("pistol", 2, 800, 0.7f));
	inv.push_back(new Gun("rifle", 8, 300, 3.9f));
	inv.push_back(new Gun("rocketLauncher", 15, 1500, 6.3f));
	inv.push_back(new Heal("HP", 20));
	curr_item = inv.begin();
}

void Engine::Inventory::delItem(std::string Name)
{
	inv.erase(
		std::remove_if(inv.begin(), inv.end(), [Name](Item* item)
			{
				if (item->getName() == Name) { delete item;  return true; }
			})
	);
	inv.shrink_to_fit();
}

void Engine::Inventory::update()
{
	for (auto iter = inv.begin(); iter != inv.end();)
	{
		auto& obj = (*iter);
		if (obj->isActive() == false)
		{
			delete obj;
			iter = inv.erase(iter);
			curr_item = inv.begin();
		}
		else
			++iter;
	}
}

Engine::Inventory::~Inventory()
{
	for (auto& iter : inv)
		delete iter;
	inv.clear();
}

void Engine::Inventory::setItemByName(std::string_view name)
{
	auto item = std::find_if(inv.begin(), inv.end(), [name](Item* item) { return item->getName() == name; });
	curr_item = item;

}

void Engine::Inventory::setItemByIndex(size_t index)
{
	assert(index > inv.size() == false && "out of range");
	curr_item = inv.begin() + index;
}

float Engine::Inventory::getWeight()
{
	float weight = 0;
	for (const auto& i : inv)
	{
		weight += i->getWeight();
	}
	return weight;
}

int Engine::Inventory::getIndexItem(std::string_view name)
{
	for (size_t i = 0; i < inv.size(); ++i)
		if (inv[i]->getName() == name)
			return i;
}

//std::vector<Gun*> Engine::Inventory::getGunItems()
//{
//	std::vector<Gun*> vec;
//	for (auto& i : inv)
//		if (i->getType() == gun)
//			vec.push_back(static_cast<Gun*>(i));
//	return vec;
//}


#pragma once
#include <vector>
#include <list>
#include <iostream>
#include <SFML\Graphics.hpp>
#include <assert.h>
#include "Object.h"

namespace Engine
{
	enum ItemType { item = 0, gun, heal };

	class Player;

	class Item : public Engine::Object
	{
	public:
		Item() = default;
		Item(std::string name) : Object(name) { type = ItemType::item; weight = 0; }
		virtual ~Item() {}
		float getWeight() const { return weight; }
		ItemType getType() const { return type; }
		std::string getType_s();
		virtual void action(Player& p) = 0;

	protected:
		ItemType type;
		sf::Clock actionClock;
		float weight;
	};

	class Gun final : public Item
	{
	private:
		float RateOfFire, damage;
		int AmmoInGun;
		sf::Clock actionClock;
	public:
		Gun() = default;
		Gun(std::string_view Name, float dmg, float rate, float w) : RateOfFire(rate), damage(dmg)
		{
			AmmoInGun = 0;
			LoadSprite(Name.data());
			weight = w;
			name = Name;
			type = ItemType::gun;
		}

		float getDamage() const { return damage; }
		int getRate() const { return (int)RateOfFire; }
		void action(Player& p) override;

	private:
		void IniGun() {

		}

		void LoadSprite(std::string_view name)  //�������� �����, ����� ��������; ���� enum ����� ��������� ����� 
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
	};

	class Heal final : public Item
	{
	private:
		float HP;
	public:
		Heal(const std::string Name, const float hp) : HP(hp) { name = Name; type = ItemType::heal; weight = 0.2f; }
		Heal() = default;
		int GetHP() const
		{
			return static_cast<int>(HP);
		}
		void action(Player& p) override;
	};

	class Inventory
	{
	private:
		std::vector<Item*> inv;
		std::vector<Item*>::iterator curr_item;
		void baseIni()
		{
			inv.push_back(new Gun("pistol", 2, 800, 0.7f));
			inv.push_back(new Gun("rifle", 8, 300, 3.9f));
			inv.push_back(new Gun("rocketLauncher", 15, 1500, 6.3f));
			inv.push_back(new Heal("HP", 20));
			curr_item = inv.begin();
		}
	public:
		Inventory()
		{
			baseIni();
		}
		~Inventory()
		{
			for (auto& iter : inv)
				delete iter;
			inv.clear();
		}

		void AddItem(Item* i) { inv.push_back(i); }

		void delItem(std::string Name)
		{
			inv.erase(
				std::remove_if(inv.begin(), inv.end(), [Name](Item* item)
			{
				if (item->getName() == Name) { delete item;  return true; }
			})
			);
			inv.shrink_to_fit();
		}

		Item* operator[](int n) { return inv[n]; }

		void update()
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

		template<class T>
		T* getItemByName(std::string Name)
		{
			auto ret = std::find_if(inv.begin(), inv.end(), [Name](Item* item)
			{
				return (item->getName() == Name) ? true : false;
			});
			return dynamic_cast<T*>(*ret);
		}

		void nextItem() { (curr_item != inv.end()) ? curr_item = inv.begin() : curr_item++; }

		std::string getItemName(size_t num) const { return (num > inv.size() - 1) ? '\0' : inv[num]->getName(); }

		template<class T>
		T* getItem(const int index)
		{
			assert((static_cast<size_t>(index) > inv.size()) == false && "out of range");
			return dynamic_cast<T*>(inv[index]);
		}

		template<class T>
		T* getCurrItem() const
		{
			return dynamic_cast<T*>(*curr_item);
		}

		void setItemByName(std::string_view name)
		{
			auto item = std::find_if(inv.begin(), inv.end(), [name](Item* item) { return item->getName() == name; });
			curr_item = item;

		}

		void setItemByIndex(size_t index)
		{
			assert(index > inv.size() == false && "out of range");
			curr_item = inv.begin() + index;
		}

		float getWeight()
		{
			float weight = 0;
			for (const auto& i : inv)
			{
				weight += i->getWeight();
			}
			return weight;
		}

		int getSize() { return inv.size(); }

		int getIndexItem(std::string_view name)
		{
			for (size_t i = 0; i < inv.size(); ++i)
				if (inv[i]->getName() == name)
					return i;
		}

		std::vector<Gun*> getGunItems()
		{
			std::vector<Gun*> vec;
			for (auto& i : inv)
				if (i->getType() == gun)
					vec.push_back(static_cast<Gun*>(i));
			return vec;
		}

		std::vector<Item*> getArrayItem() { return inv; }
	};
}


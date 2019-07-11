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
	protected:
		ItemType type;
		sf::Clock actionClock;
		float weight;

	public:
		Item() = default;
		Item(std::string name) : Object(name) { type = ItemType::item; weight = 0; }
		virtual ~Item() {}
		virtual void action(Player& p) = 0;

		float getWeight() const { return weight; }
		ItemType getType() const { return type; }
		std::string getType_s();
	};

	class Gun final : public Item
	{
	private:
		float RateOfFire, damage;
		int AmmoInGun;
		sf::Clock actionClock;

		void IniGun() {}
		void LoadSprite(std::string_view name);
	public:
		Gun() = default;
		Gun(std::string_view Name, float dmg, float rate, float w);

		float getDamage() const { return damage; }
		int getRate() const { return (int)RateOfFire; }
		void action(Player& p) override;
	};

	class Heal final : public Item
	{
	private:
		float HP;
	public:
		Heal(const std::string Name, const float hp) : HP(hp) { name = Name; type = ItemType::heal; weight = 0.2f; }
		Heal() = default;

		int GetHP() const { return static_cast<int>(HP); }
		void action(Player& p) override;
	};

	class Inventory
	{
	private:
		std::vector<Item*> inv;
		std::vector<Item*>::iterator curr_item;

		void baseIni();
	public:
		Inventory() { baseIni(); }
		~Inventory();

		void setItemByName(std::string_view name);
		void setItemByIndex(size_t index);
		void AddItem(Item* i) { inv.push_back(i); }
		void delItem(std::string Name);
		void update();
		void nextItem() { (curr_item != inv.end()) ? curr_item = inv.begin() : curr_item++; }

		auto begin() { return inv.begin(); }
		auto end() { return inv.end(); }
		std::string getItemName(size_t num) const { return (num > inv.size() - 1) ? '\0' : inv[num]->getName(); }
		float getWeight();
		int getSize() { return inv.size(); }
		int getIndexItem(std::string_view name);

		auto getItemsByType(ItemType type)
		{
			std::vector<Item*> out;
			std::copy_if(inv.begin(), inv.end(), out.begin(), [type](const Item* i)
				{
					return i->getType() == type;
				});
			return out;
		}

		template<class T>
		T* getItem(const int index)
		{
			assert(!(static_cast<size_t>(index) > inv.size()) && "out of range");
			return dynamic_cast<T*>(inv[index]);
		}
		template<class T>
		T* getCurrItem() const
		{
			return dynamic_cast<T*>(*curr_item);
		}
		template<class T>
		T* getItemByName(std::string_view Name)
		{
			auto ret = std::find_if(inv.begin(), inv.end(), [Name](Item* item)
				{
					return (item->getName() == Name) ? true : false;
				});
			return dynamic_cast<T*>(*ret);
		}
		Item* operator[](int n) { return inv[n]; }
	};
}


#pragma once
#include <vector>
#include <list>
#include <iostream>
#include <SFML\Graphics.hpp>
#include "Object.h"
#include <assert.h>
namespace Engine
{
	enum ItemType { item = 0, gun, heal };
	class Item : public Engine::Object
	{
	public:
		Item() = default;
		Item(std::string name) : Object(name) { type = ItemType::item; weight = 0; }
		virtual ~Item() {};
		float getWeight() { return weight; }
		int getInt() { return 1; } //LoL?
		ItemType getType() { return type; }
		std::string getType_s();
		void update() { dw_o.draw(name); };
	protected:
		ItemType type;
		float weight;
	};

	class Gun : public Item
	{
	private:
		float RateOfFire, damage;
		int AmmoInGun;
	public:
		Gun() = default;
		Gun(std::string Name, float dmg, float rate, float w) : RateOfFire(rate), damage(dmg)
		{
			AmmoInGun = 0;
			LoadSprite(Name);
			weight = w;
			name = Name;
			type = ItemType::gun;
		}

		int getDamage() const { return (int)damage; }
		int getRate() const { return (int)RateOfFire; }

	private:
		void IniGun() {

		}

		void LoadSprite(std::string name)  //Выглядит плохо, нужно заменить; Даже enum будет выглядеть лучше 
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
				texture.loadFromFile("Inventor: image/ТАКОГО ФАЙЛА НЕТ"); //ERR
			}
		}
	};

	class Heal : public Item
	{
	private:
		float HP;
	public:
		Heal(std::string Name, float hp) : HP(hp) { name = Name; type = ItemType::heal; weight = 0.2; }
		Heal() = default;
		int GetHP() { return (int)HP; }
	};

	class Inventory
	{
	private:
		int Count = 0;
		std::vector<Item*> inv;
		std::vector<Item*>::iterator iter;
		void baseIni()
		{
			inv.push_back(new Gun("pistol", 2, 5, 0.7));
			inv.push_back(new Gun("rifle", 8, 2, 3.9));
			inv.push_back(new Gun("rocketLauncher", 15, 10, 6.3));
			inv.push_back(new Heal("HP", 20));
		}
	public:
		Inventory() { baseIni(); }
		~Inventory()
		{
			for (auto iter = inv.begin(); iter != inv.end(); ++iter)
				delete *iter;
			inv.clear();
		}

		void AddItem(Item* i) { inv.push_back(i); }

		void delItem(std::string Name)
		{
			auto eraseIter = std::remove_if(inv.begin(), inv.end(), [Name](Item* item)
			{
				if (item->getName() == Name)
					return item;
			});
			delete *eraseIter;
			inv.erase(eraseIter);
		}

		Item* operator[](int n) { return inv[n]; }

		void update()
		{
			for (auto& i : inv)
			{
				i->update();
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

		void nextItem() { (Count >= inv.size() - 1) ? Count = 0 : Count++; }

		std::string getItemName(int num) const { return (num > inv.size() - 1) ? '\0' : inv[num]->getName(); }

		template<class T>
		T* getItem(int index)
		{
			//assert(index > inv.size(), "out of range");
			return (index > inv.size() - 1) ? nullptr : dynamic_cast<T*>(inv[index]);
		}

		template<class T>
		T* getCurrItem() const { return dynamic_cast<T*>(inv[Count]); }

		int getCurrCount() { return Count; }

		void setCount(int i) { Count = i; }

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
			for (auto i = 0; i < inv.size(); ++i)
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


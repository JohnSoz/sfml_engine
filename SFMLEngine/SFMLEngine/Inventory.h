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
		Gun(std::string_view Name, float dmg, float rate, float w) : RateOfFire(rate), damage(dmg)
		{
			AmmoInGun = 0;
			LoadSprite(Name.data());
			weight = w;
			name = Name;
			type = ItemType::gun;
		}

		int getDamage() const { return (int)damage; }
		int getRate() const { return (int)RateOfFire; }

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
				texture.loadFromFile("Inventor: image/������ ����� ���"); //ERR
			}
		}
	};

	class Heal : public Item
	{
	private:
		float HP;
	public:
		Heal(std::string Name, float hp) : HP(hp) { name = Name; type = ItemType::heal; weight = 0.2f; }
		Heal() = default;
		int GetHP() { return (int)HP; }
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
			//inv.push_back(new Heal("HP", 20));
			curr_item = inv.begin();
		}
	public:
		Inventory() { baseIni(); }
		~Inventory()
		{
			for (auto & iter : inv)
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

		void nextItem() { (curr_item != inv.end()) ? curr_item = inv.begin() : curr_item++; }

		std::string getItemName(int num) const { return (num > inv.size() - 1) ? '\0' : inv[num]->getName(); }

		template<class T>
		T* getItem(int index)
		{
			assert((index > inv.size()) == false, "out of range");
			return dynamic_cast<T*>(inv[index]);
		}

		template<class T>
		T* getCurrItem() const { return dynamic_cast<T*>(*curr_item); }

		void setItemByName(std::string_view name)
		{
			auto item = std::find_if(inv.begin(), inv.end(), [name](Item* item) { return item->getName() == name; });
			if ((*item)->getType() != ItemType::heal)
			{
				curr_item = item;
			}
			else
			{

			}
		}

		void setItemByIndex(size_t index)
		{
			assert((index > inv.size()) == false, "out of range");
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


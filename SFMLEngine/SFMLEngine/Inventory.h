#pragma once
#include <vector>
#include <list>
#include <iostream>
#include <SFML\Graphics.hpp>
#include "Object.h"
#include <typeinfo> 
namespace Engine
{

	enum ItemType { item = 0, gun, heal };
	class Item : public Engine::Object
	{
	public:
		Item() = default;
		Item(std::string name) { type = ItemType::item; weight = 0; }
		virtual ~Item() {};

		float getWeight() { return weight; }
		int getInt() { return 1; } //LoL?
		ItemType getType() { return type; }
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

		void LoadSprite(std::string name)  //�������� �����, ����� ��������; ���� enum ����� ��������� ����� 
		{
			if (name == "M9")
			{
				texture.loadFromFile("images/Pistolet.png");
				sprite.setTexture(texture);
			}
			else if (name == "AR-15")
			{
				texture.loadFromFile("images/AK_47.png");
				sprite.setTexture(texture);
			}
			else if (name == "RPG")
			{
				texture.loadFromFile("images/RPG.png");
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
		int Size = 0;
	public:
		~Inventory()
		{
			for (auto iter = inv.begin(); iter != inv.end(); ++iter)
				delete *iter;
			inv.clear();
		}
		/////////////////////////
		///������� ������������� ������� 
		/////////////////////////
		void baseIni()
		{
			inv.push_back(new Gun("M9", 2, 2, 0.7));
			inv.push_back(new Gun("AR-15", 8, 12, 3.9));
			inv.push_back(new Gun("RPG", 15, 1, 6.3));
			inv.push_back(new Heal("HP", 20));
			Size = inv.size();
		}

		/////////////////////////
		///��������� ������� � ����� �������
		/////////////////////////
		void AddItem(Item* i) { inv.push_back(i); Size++; }

		/////////////////////////
		///������� ������� ������� �� ��� �����
		/////////////////////////
		void delItem(std::string Name)
		{
			for (iter = inv.begin(); iter != inv.end(); iter++)
				if ((*iter)->getName() == Name) {
					inv.erase(iter);
					break;
				}
			Size = inv.size();
		}

		int getNumItem(std::string Name)
		{
			for (int i = 0; i < Size; i++)
			{
				if (inv[i]->getName() == Name)
				{
					return i;
				}
			}
		}

		void nextItem() { (Count >= inv.size() - 1) ? Count = 0 : Count++; }

		/////////////////////////
		///���������� ��� �������� �� ��� id
		/////////////////////////
		std::string  getItemName(int num) const { return (num > Size - 1) ? '\0' : inv[num]->getName(); }

		/////////////////////////
		///���������� ��������� �� Item �� ��� id
		/////////////////////////
		template<class T>
		T* getItem(int index)
		{
			//assert(Index > Size);
			return (index > Size - 1) ? nullptr : dynamic_cast<T*>(inv[index]);
		}
		/////////////////////////
		///���������� ��������� �� ������� �������
		/////////////////////////
		Item* getCurrItem() const { return inv[Count]; }

		/////////////////////////
		///���������� id �������� ��������
		/////////////////////////
		int getCurrCount() { return Count; }

		/////////////////////////
		///������ id �������� ��������
		/////////////////////////
		void setCount(int i) { Count = i; }

		/////////////////////////
		///������ ������ ������� ���������
		/////////////////////////
		void setSize(int i) { Size = i; }

		/////////////////////////
		///���������� ������ ������� ���������
		/////////////////////////
		int getInvSize() { return Size; }

		/////////////////////////
		///���������� ��� ���� item � ���������
		/////////////////////////
		float getWeight()
		{
			float weight = 0;
			for (const auto& i : inv)
			{
				weight += i->getWeight();
			}
			return weight;
		}

		/////////////////////////
		///���������� ������ 
		/////////////////////////
		std::vector<Item*> getArrayItem() { return inv; }
	};
}


#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include <atomic>
#include <bitset>
#include <vector>
#include <array>

namespace Engine
{
	enum ObjectType
	{
		OEntity = 1,
		OPawn,
		OActor
	};

	class Component;
	class Entity;

	using ComponentID = size_t;

	inline ComponentID getComponentTypeID()
	{
		static ComponentID lastId = 0;
		return lastId++;
	}

	template<typename T> 
	inline ComponentID getComponentTypeID() noexcept
	{
		static ComponentID typeId = getComponentTypeID();
		return typeId;
	}

	constexpr size_t maxComponent = 32;
	using ComponentBitSet = std::bitset<maxComponent>;
	using ComponentArray = std::array<Component*, maxComponent>;

	class Component 
	{
	public:
		Entity* entity;
		virtual ~Component() {}

		virtual void Init() {}
		virtual void update() {}
		virtual void draw() {}
	};

	class Entity
	{
	protected:
		ObjectType type;
		Vector2D position;
		Rectangle rectangle;
		sf::Sprite sprite;
		sf::Texture texture;
		std::string name;
		int id = 0;
		bool active = true;
		std::vector<Component*> components;

		ComponentArray componentArray;
		ComponentBitSet componentBitSet;
	private:
		
	public:
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(Entity&&) = default;

		Entity(Vector2D POSITION, std::string NAME);
		Entity(sf::Image& IMAGE, Vector2D POSITION, std::string NAME);
		Entity(sf::Image& IMAGE, sf::IntRect r, Vector2D pos, std::string name);
		virtual ~Entity();

		virtual void update(float time) 
		{
			for (auto &c : components) c->update();
			//for (auto &c : components) c->draw();
		};

		bool isActive() const { return active; }
		Rectangle getRect() const { return rectangle; }
		Vector2D getPos()const { return position; }
		ObjectType getType() const { return type; }
		void destroy() { active = false; }

		template<typename T>
		bool hasComponent() const 
		{
			return componentBitSet[getComponentTypeID<T>()];
		}

		template<typename T,typename... Argc>
		T& addComponent(Argc&&... argc) 
		{
			T* c(new T(std::forward<Argc>(argc)...));
			c->entity = this;
			components.push_back(c);
			componentArray[getComponentTypeID<T>()] = c;
			componentBitSet[getComponentTypeID<T>()] = true;
			c->Init();
			return *c;
		}
		template<typename T>
		T* getComponent() const
		{
			return static_cast<T*>(components[getComponentTypeID<T>()]);
		}

		friend class ObjectHandler;
	};

	class PositionComponent :public Component
	{
	private:
		int x, y = 0;
	public:
		void Init() override 
		{
			//SetPos();
		}
		void update() override
		{
			std::cout << "X = " << x <<std::endl;
		}
		void SetPos() {
			x = y = 12;
		}
	};

	class Actor : public Entity
	{
	protected:
	public:
		Actor() = delete;
		Actor(sf::Image& IMAGE, Vector2D POSITION, Rectangle rect, std::string NAME) : Entity(IMAGE, POSITION, NAME)
		{
			rectangle = rect;
			sprite.setTextureRect(rectangle.getSfmlRect());
		}
		//void update(float time) override { sprite.setPosition(position.GetSfmlVector()); }
	};

}


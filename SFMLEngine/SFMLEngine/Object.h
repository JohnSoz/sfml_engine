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
	class World;

	class Object
	{
	protected:

	public:
		Object() = delete;

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

	public:
		Entity() = default;

		Entity(const Entity&) = default;

		Entity(Entity&&) = default;


		Entity(Vector2D POSITION, std::string NAME);

		Entity(sf::Image& IMAGE, Vector2D POSITION, std::string NAME);

		Entity(sf::Image& IMAGE, sf::IntRect r, Vector2D pos, std::string name);

		virtual ~Entity();


		virtual void update(float time) = 0;

		void SetPos(int x, int y) { position.x = x; position.y = y; }

		void RotateToMouse(float speed, sf::RenderWindow& window);

		bool isActive() const { return active; }

		const Rectangle &getRect() { return rectangle; }

		const Vector2D &getPos() { return position; }

		const ObjectType &getType() { return type; }

		void destroy() { active = false; }

		friend class ObjectHandler;
	};

	enum Direction
	{
		Up = 1, Down, Left, Right, State
	};

	class Actor : public Entity
	{
	protected:
		Vector2D velocity;
		bool isWalk =    false;
		float speed =    0;
		float energy =   0.003;
		float maxSpeed = 0.4;
		float friction = 0.0009;
		Direction direction = Direction::State;
	public:
		Actor() = delete;
		Actor(sf::Image& IMAGE, Vector2D POSITION, Rectangle rect, std::string NAME) : Entity(IMAGE, POSITION, NAME)
		{
			rectangle = rect;
			sprite.setTextureRect(rectangle.getSfmlRect());
		}
		void handleEvent(sf::Event& e);
		void update(float time) override;
		//void update(float time) { sprite.setPosition(position.GetSfmlVector()); }
	};

}


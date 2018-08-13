#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include <atomic>
#include <bitset>
#include <vector>
#include <array>
#include "Level.h"

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
		Rectangle localRectangle;
		Rectangle globalRectangle;
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

		bool isActive() const { return active; }

		Rectangle &getRect() { return localRectangle; }

		Vector2D &getPos() { return position; }

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
		std::vector<ObjectLevel> obj;
		Vector2D velocity;
		bool isWalk = false;
		bool life = false;
		float lives = 100;
		float armor = 100;
		float speed = 0;
		float energy = 0.005;
		float maxSpeed = 0.3;
		float friction = 0.005;
		float  CurrAngle = sprite.getRotation(), Radian = 0, LastAngle = 0;
		sf::Vector2f originOffset = {};
		Direction direction = Direction::State;
		sf::RenderWindow* window;
	public:
		Actor() = delete;
		Actor(sf::Image& IMAGE, Vector2D POSITION, Rectangle rect, std::string NAME, sf::RenderWindow& w, Level& lvl) : Entity(IMAGE, POSITION, NAME)
		{
			localRectangle = rect;
			globalRectangle = Rectangle(position.x, position.y, position.x + rect.w, position.y + rect.y);
			obj = lvl.GetAllObjects();
			sprite.setOrigin(90, 120);
			originOffset = { 90,120 };
			sprite.setTextureRect(localRectangle.getSfmlRect_i());
			window = &w;
		}
		void handleEvent(sf::Event& e);
		void checkClashes(Vector2D pos);
		void RotateToMouse(float speed, sf::RenderWindow& window);
		void update(float time) override;
		void getDamage(float dmg);
		//void update(float time) { sprite.setPosition(position.GetSfmlVector()); }
	};

}


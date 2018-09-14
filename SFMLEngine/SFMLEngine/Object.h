#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Level.h"
#include <vector>
#include <array>
#include "imgui.h"
#include "imgui-sfml.h"
#include "AnimationManager.h"
#include <stdio.h>
#define SHOW(a) std::cout << #a << ": " << (a) << std::endl
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
		ObjectType type;
		sf::Vector2f position;
		sf::Texture texture;//?
		sf::Sprite sprite;//?

		std::string name;
		bool IsActive;
	public:
		Object() = default;
		~Object() = default;
		Object(sf::Vector2f pos, std::string name);
		Object(sf::Vector2f pos, ObjectType t, std::string name);
		bool isActive() const { return IsActive; }
		void destroy() { IsActive = false; }
		std::string getName() { return name; }

		friend class Debug_Object;
	};

	class Entity : public Object
	{
	protected:
		sf::IntRect      localRectangle;
		sf::FloatRect    globalRectangle;
		sf::FloatRect    debugRectangle;
		int         id = 0;

	public:
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(Entity&&) = default;
		Entity(sf::Vector2f POSITION, std::string NAME);
		Entity(sf::Image& IMAGE, sf::Vector2f POSITION, std::string NAME);
		Entity(sf::Image& IMAGE, sf::IntRect r, sf::Vector2f pos, std::string name);
		virtual ~Entity();


		virtual void update(float time) = 0;
		void SetPos(int x, int y) { position.x = x; position.y = y; }

		sf::IntRect getRect() { return localRectangle; }
		const sf::Vector2f& getPos() { return position; }
		ObjectType getType() { return type; }

		std::pair<sf::FloatRect*, sf::FloatRect*> getDebugRect() { return std::make_pair(&globalRectangle, &debugRectangle); }

		friend class ObjectHandler;
	};

	enum Direction
	{
		Up = 1, Down, Left, Right, State
	};

	class Actor;

	class Debug
	{
	protected:
		static inline ImVec2 size = {400,400};
		static void ShowHelpMarker(const char* desc);
	public:
	};

	class Debug_Object : public Debug
	{
	public:
		static void objectInfo(bool *open, Object& a);
		void draw(bool *open, Object& a);
	};

	class Debug_Actor : public Debug_Object
	{
	public:
		static void actorInfo(bool *open, Actor& a);
		void draw(bool *open, Actor& a);
	};

	class Actor : public Entity, public Debug_Actor
	{
	protected:
		std::vector<ObjectLevel> obj;
		sf::Vector2f velocity;
		AnimationManager animManager;

		bool showDebugConsole = false;
		bool isWalk = false;
		bool isCollision = false;
		float lives = 100;
		float armor = 100;
		float speed = 0;
		float energy = 0.005;
		float friction = 0.005;
		float maxSpeed = 0.3;
		float CurrAngle = sprite.getRotation(), Radian = 0, LastAngle = 0;

		sf::Clock Pressclock;
		sf::Vector2f originOffset = {};
		Direction direction = Direction::State;
		sf::RenderWindow* window;

	public:
		Actor() = delete;
		Actor(sf::Image& IMAGE, sf::Vector2f POSITION, sf::IntRect rect, std::string NAME, sf::RenderWindow& w, Level& lvl) : Entity(IMAGE, POSITION, NAME)
		{
			animManager.LoadAnimation_x("Move.xml");
			localRectangle = rect;
			globalRectangle = sf::FloatRect(position.x, position.y, position.x + rect.width, position.y + rect.top);
			obj = lvl.GetAllObjects();
			originOffset = { 90,120 };
			sprite.setOrigin(originOffset);
			sprite.setTextureRect(localRectangle);
			window = &w;
		}
		void handleEvent(sf::Event& e);
		void checkClashes(sf::Vector2f pos);
		void RotateToMouse(float speed, sf::RenderWindow& window);
		void update(float time) override;
		void getDamage(float dmg);

		friend class Debug_Actor;
	};
}


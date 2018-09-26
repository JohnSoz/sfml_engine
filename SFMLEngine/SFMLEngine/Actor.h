#pragma once
#include "Entity.h"
#include "AnimationManager.h"
#include "Inventory.h"
#include "imgui.h"
#include "imgui-sfml.h"
#include "Level.h"
#include <typeinfo>
namespace Engine
{
	class Actor;

	class Debug
	{
	protected:
		static inline ImVec2 size = { 400,400 };
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
		Inventory inventory;
		AnimationManager animManager;

		bool showDebugConsole;
		bool isWalk;
		bool isCollision;

		float lives, armor;
		float speed, energy, friction, maxSpeed;
		float CurrAngle = sprite.getRotation(), Radian, LastAngle;

		sf::Clock Pressclock;
		sf::Vector2f originOffset = {};
		Direction direction = Direction::State;
		sf::RenderWindow* window;

	public:
		Actor() = delete;
		Actor(sf::Image& IMAGE, sf::Vector2f POSITION, sf::IntRect rect, std::string NAME, sf::RenderWindow& w, Level& lvl) : Entity(IMAGE, POSITION, NAME)
		{
			inventory.baseIni();
			animManager.LoadAnimation_x("Move.xml");
			lives = armor = 100;
			speed = 0;
			energy = friction = 0.005;
			maxSpeed = 0.3;
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
#pragma once
#include "Actor.h"
namespace Engine
{
	class Player final : public Actor
	{
	private:
		sf::Vector2f velocity;
		Inventory inventory;
		AnimationManager animManager;

		bool isWalk;
		bool isCollision;

		float lives, armor;
		float speed, energy, friction, maxSpeed;
		float scale = 0.5;
		float CurrAngle = sprite.getRotation(), LastAngle;

		Direction state = Direction::State;
		sf::Transformable PointOfFire;
	public:
		//Player(sf::Image& IMAGE, sf::Vector2f POSITION, sf::IntRect rect, std::string NAME, sf::RenderWindow& w, Level& lvl) :Actor(IMAGE, POSITION, rect, NAME, w, lvl)
		//{

		//}
		void update(float time) override {}
		void handleEvent(sf::Event& e) {}
		~Player() = default;
	};
}

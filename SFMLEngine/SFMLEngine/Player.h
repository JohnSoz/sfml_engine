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
	bool isShoot;

	float lives, armor;
	float speed, energy, friction, maxSpeed;
	float scale = 0.5;
	float CurrAngle = sprite.getRotation(), LastAngle;

	Direction state = Direction::State;
	sf::Transformable PointOfFire;

public:
	Player(sf::Image &IMAGE, sf::Vector2f POSITION, std::string NAME, sf::RenderWindow &w, Level &lvl)
			: Actor(IMAGE, POSITION, NAME, w, lvl)
	{
		isWalk = isCollision = isShoot = false;
	}
	void RotateToMouse(float speed, sf::RenderWindow &w) {}
	void update(float time) override {}
	void start() override {}
	void CollisionUpdate(Entity *entity) override {}
	void handleEvent(sf::Event &e) override {}
	~Player() = default;
};
} // namespace Engine

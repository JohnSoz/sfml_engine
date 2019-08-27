#pragma once
#include "BaseObject.h"
#include "ColliderSprite.h"
#include "AnimationManager.h"
namespace Engine
{
	class Pawn : public BaseObject, public ColliderSprite
	{
	private:
		AnimationManager animManager;
		sf::RenderWindow* window;
	public:
		Pawn(std::string name, std::string_view pathToTexture, std::string_view animation, sf::RenderWindow& w) :
			BaseObject(name), ColliderSprite(pathToTexture)
		{
			window = &w;
			animManager.loadAnimation_x(animation, *window);
		}

		virtual void fixedUpdate() = 0;
		virtual void CollisionUpdate(Pawn* objarray) {}
		virtual void handleEvent(sf::Event& e) = 0;
		virtual void isKeyPressed() = 0;
		virtual void update(float time) = 0;
		virtual void checkClashes(float time) = 0;
	};
}

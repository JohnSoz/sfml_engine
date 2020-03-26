#pragma once
#include "BaseObject.h"
#include "ColliderSprite.h"
#include "AnimationManager.h"
#include "Level.h"
namespace Engine
{
	class Pawn : public BaseObject, public ColliderSprite
	{
	private:
		AnimationManager  animManager;
		sf::RenderWindow* window;
		std::vector<ObjectLevel> levelObjects;
		DirectionX               direction;
		DirectionY               directionY;

		void updateSprite()
		{
			auto currAnim = animManager.getCurrAnimation<AnimationXml>();
			setRect(currAnim->rect);
			setOrigin(currAnim->origin);
			setScale({ currAnim->scale, currAnim->scale });
		}
	public:
		Pawn(std::string name, std::string_view pathToTexture, std::string_view animation, sf::RenderWindow& window, Level& lvl, sf::Vector2f position);

		virtual void fixedUpdate() = 0;
		virtual void collisionUpdate(Pawn* objarray) {}
		virtual void handleEvent(sf::Event& e) = 0;
		virtual void isKeyPressed() = 0;
		virtual void update(float time) = 0;
		virtual void checkClashes(float time) = 0;
	};
}

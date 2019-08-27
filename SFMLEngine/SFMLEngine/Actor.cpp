#include "Actor.h"
#include "Math.h"
using sf::Keyboard;

void Engine::Actor::updateSprite()
{
	auto currAnim = animManager.getCurrAnimation<AnimationXml>();
	localRectangle = currAnim->rect;
	originOffset = currAnim->origin;
	scale = currAnim->scale;
	collider.setScale(scale);
	collider.setCenter(originOffset);
	//sprite.setScale(sf::Vector2f(scale, scale));
	sprite.setOrigin(originOffset);
	sprite.setTextureRect(localRectangle);
	sprite.setPosition(position);
}

Engine::Actor::Actor(Level& lvl)
{
	setType(OActor);
	obj = lvl.GetAllObjects();
	direction = Left;
}

Engine::Actor::Actor(sf::Vector2f POSITION, std::string NAME, sf::RenderWindow& w, Level& lvl, std::string_view animation) : Entity(POSITION, NAME)
{
	direction = Left;
	setType(OActor);
	//globalRectangle = sf::FloatRect(position.x, position.y, position.x + localRectangle.width, position.y + localRectangle.top);
	obj = lvl.GetAllObjects();
	window = &w;
	animManager.loadAnimation_x(animation, w);
	auto object_level = *std::find_if(obj.begin(), obj.end(), [](auto Obj) { return (Obj.name == "PlayerSpawn"); });
	position = sf::Vector2f(object_level.rect.left + object_level.rect.width / 2, object_level.rect.top + object_level.rect.height / 2);
	setTexture(animManager.texture);
	updateSprite();
}

void Engine::Actor::CollisionUpdate(Entity* entity)
{
	auto playerRect = Rectangle::fromSfmlRect(sprite.getGlobalBounds());
	auto objectRect = Rectangle::fromSfmlRect(entity->getRect());
	auto offset = Rectangle::GetIntersectionDepth(playerRect, objectRect);
	position = position + offset.GetSfmlVector();
}

void Engine::Actor::update(float time) {}
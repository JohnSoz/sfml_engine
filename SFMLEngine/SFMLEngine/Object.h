#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "BaseObject.h"
namespace Engine
{
	class Object : public BaseObject
	{
	protected:
		std::string pathToSprite;
		sf::Vector2f position;
		sf::Texture texture;
		sf::Sprite sprite;
		void setTexture(const sf::Texture& tex);
	public:
		Object() = default;
		~Object() = default;
		Object(const Object&) = default;
		Object(sf::Vector2f position, std::string name);
		Object(sf::Image image, sf::Vector2f position, std::string name);

		const sf::Sprite&  getSprite()  const { return sprite; }
		const sf::Texture& getTexture() const { return texture; }

		friend class DebugWindow;
		friend auto meta::registerMembers<Object>();
	};

}

namespace meta
{
	template <>
	inline auto registerMembers<Engine::Object>()
	{
		return members(
			member("pathToSprite", &Engine::Object::pathToSprite),
			member("position", &Engine::Object::position),
			member("sprite", &Engine::Object::sprite)
		);
	}
}
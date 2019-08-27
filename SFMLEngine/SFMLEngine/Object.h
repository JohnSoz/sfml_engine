#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "BaseObject.h"
namespace Engine
{
	/// Enumeration of the possible directions along the x-axis
	enum DirectionX { Left = 1, Right };
	/// Enumeration of the possible directions along the y-axis
	enum DirectionY { Up = 1, onGround };

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
		Object(sf::Vector2f pos, std::string name);
		Object(sf::Image img, sf::Vector2f pos, std::string name);

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
			member("pathToSprite", &Engine::Object::pathToSprite)
		);
	}
}
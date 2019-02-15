#pragma once

#include <SFML/Graphics.hpp>
#include "LogConsole.h"
#include <vector>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include <MetaStuff/Meta.h>
#include "imgui.h"
#include "imgui-SFML.h"
#include "staticVariable.h"

using namespace meta;
namespace Engine
{
	/// Enumeration of possible types for objects
	enum ObjectType { None = 0, OEntity, OPawn, OActor };
	/// Enumeration of the possible directions along the x-axis
	enum DirectionX { Left = 1, Right };
	/// Enumeration of the possible directions along the y-axis
	enum DirectionY { Up = 1, Down };
	/*! 
	\brief This is an enum class
	\details 
	*/
	enum valueType { INT, BOOL, FLOAT, STRING, VECTOR2F, SPRITE, ANIMATIONMANAGER, RECT_i };

	class Object;
	template<class T>
	class DebugWindows
	{
	public:
		DebugWindows() = default;
		~DebugWindows() = default;
		void set(T* p)
		{
			ptr = p;
			static_assert(meta::isRegistered<T>(), "Err");
		}
		void draw(std::string nameTree, bool isFirstDraw = false, bool isLastDraw = false);
	private:
		template<class Type>
		valueType checkType();
		T* ptr;
		sf::Vector2f size = { 400,400 };
	};

	/*!
	\brief The basic essence of the game world
	*/
	class Object
	{
	protected:
		ObjectType type; 
		sf::Vector2f position; //<Object position in game coordinates
		sf::Texture texture; //<The texture of the current object
		sf::Sprite sprite; //<Sprite of the current object
		std::string name; //<Current object name /warning Must be unique
		bool IsActive; //<The flag indicates whether the current object is alive /warning If the value is false, the object is removed from the game world
		DebugWindows<Object> dw_o;
	public:
		Object();
		explicit Object(std::string);
		Object(sf::Vector2f pos, std::string name);
		Object(sf::Vector2f pos, ObjectType t, std::string name);
		Object(sf::Image img, sf::Vector2f pos, ObjectType t, std::string name);
		~Object() = default;

		bool isActive()                 const noexcept { return IsActive; }
		const std::string& getName()    const noexcept { return name; }
		const sf::Sprite& getSprite()   const noexcept { return sprite; }
		const sf::Texture& getTexture() const noexcept { return texture; }
		void destroy() { IsActive = false; }

		friend bool operator==(const Object& obj, const Object& obj2);

		friend class DebugWindow;
		friend auto meta::registerMembers<Object>();
	};

	template<class T>
	template<class Type>
	valueType DebugWindows<T>::checkType()
	{
		if (std::is_same<Type, std::string>::value)
			return STRING;
		else if (std::is_same<Type, bool>::value)
			return BOOL;
		else if (std::is_same<Type, sf::Vector2f>::value)
			return VECTOR2F;
		else if (std::is_same<Type, sf::IntRect>::value)
			return RECT_i;
		else if (std::is_same<Type, sf::Sprite>::value)
			return SPRITE;
		else if (is_integral<Type>::value)
			return INT;
		else if (std::is_floating_point<Type>::value)
			return FLOAT;
		else if (std::is_enum<Type>::value)
			return INT;
		else
			Console::AppLog::addLog("UB", Console::logType::error);
	}

	template<class T>
	inline void DebugWindows<T>::draw(std::string nameTree, bool isFirstDraw, bool isLastDraw)
	{
		if (Engine::VStaticContainer::ShowDebugWindow)
		{
			if (ImGui::Begin("DebugWindows", NULL))
			{
				ImGui::SetWindowSize(size);
				if (isFirstDraw)
				{
					ImGui::Text(ptr->getName().c_str());
				}
				if (ImGui::TreeNode(nameTree.c_str()))
				{
					meta::doForAllMembers<T>(
						[&](const auto & member)
					{
						std::string field = "";
						switch (checkType<decltype(member.getCopy(*ptr))>())
						{
						case Engine::SPRITE:
							if (ImGui::TreeNode("Object Sprite"))
							{
								auto spr = (Object*)ptr;
								int size_x = spr->getSprite().getTextureRect().width;
								int size_y = spr->getSprite().getTextureRect().height;
								ImGui::Text("%.0fx%.0f", size_x, size_y);
								ImGui::Image(*spr->getSprite().getTexture(), sf::Vector2f(size_x, size_y), (sf::FloatRect)spr->getSprite().getTextureRect());
								ImGui::TreePop();
							}
							break;
						case Engine::INT:
							field = (std::string)member.getName() + ": %.i";
							ImGui::Text(field.c_str(), member.get(*ptr));
							break;
						case Engine::RECT_i:
						{
							field = (std::string)member.getName() + ": (%i, %i, %i, %i)";
							auto rect = meta::getMemberValue<sf::IntRect>(*ptr, member.getName());
							ImGui::Text(field.c_str(), rect.left, rect.top, rect.width, rect.height);
						}
						break;
						case Engine::BOOL:
							field = (std::string)member.getName() + ": %i";
							ImGui::Text(field.c_str(), member.get(*ptr));
							break;
						case Engine::FLOAT:
							field = (std::string)member.getName() + ": %.4f";
							ImGui::Text(field.c_str(), member.get(*ptr));
							if (ImGui::BeginPopupContextItem(field.c_str()))
							{
								float changedValue = meta::getMemberValue<float>(*ptr, member.getName());
								if (ImGui::Selectable("Set to zero")) changedValue = 0.0f;
								if (ImGui::Selectable("Set to default")) changedValue = 0.005f;
								ImGui::PushItemWidth(200);
								std::string name = "#" + (std::string)member.getName();
								ImGui::DragFloat(name.c_str(), &changedValue, 0.001f, 0.001f, 1.f);
								meta::setMemberValue<float>(*ptr, member.getName(), changedValue);
								ImGui::PopItemWidth();
								ImGui::EndPopup();
							}
							break;
						case Engine::STRING:
							field = (std::string)member.getName() + ": " + meta::getMemberValue<std::string>(*ptr, member.getName());
							ImGui::Text(field.c_str());
							break;
						case Engine::VECTOR2F:
						{
							field = (std::string)member.getName() + ": (%.2f %.2f)";
							auto vec = meta::getMemberValue<sf::Vector2f>(*ptr, member.getName());
							ImGui::Text(field.c_str(), vec.x, vec.y);
						}
						break;
						default:
							break;
						}
					}
					);
					ImGui::TreePop();
				}
				if (isLastDraw)
				{
					ImGui::Spacing();
					ImGui::Separator();
					ImGui::Spacing();
				}
			}
			ImGui::End();
		}
	}

}

namespace meta
{
	template <>
	inline auto registerMembers<Engine::Object>()
	{
		return members(
			member("name", &Engine::Object::name),
			//member("type", &Engine::Object::type),
			member("sprite", &Engine::Object::sprite),
			member("IsActive", &Engine::Object::IsActive),
			member("position", &Engine::Object::position)
		);
	}
}
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <MetaStuff/Meta.h>
#include "imgui.h"
#include "imgui-SFML.h"
#include "staticVariable.h"

using namespace meta;
namespace Engine
{
	enum ObjectType { None = 0, OEntity, OPawn, OActor };
	enum Direction { Up = 1, Down, Left, Right, State };
	enum valueType { INT, BOOL, FLOAT, STRING, VECTOR2F, SPRITE, ANIMATIONMANAGER };

	class Object;
	template<class T>
	class DebugWindows
	{
	public:
		DebugWindows() {};
		~DebugWindows()
		{
		}
		void set(T* p)
		{
			ptr = p;
			static_assert(meta::isRegistered<T>(), "Err");
		}
		void draw(std::string nameTree, bool isFirstDraw = false)
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
					if (ImGui::TreeNode((void*)ptr, nameTree.c_str()))
					{
						meta::doForAllMembers<T>(
							[&](const auto& member)
						{
							std::string field = "";
							switch (checkType<decltype(member.getCopy(*ptr))>())
							{
							case Engine::SPRITE:
								if (ImGui::TreeNode("Object Sprite"))
								{
									auto spr = (Object*)ptr;
									float size_x = spr->getSprite().getTextureRect().width;
									float size_y = spr->getSprite().getTextureRect().height;
									ImGui::Text("%.0fx%.0f", size_x, size_y);
									ImGui::Image(*spr->getSprite().getTexture(), sf::Vector2f(size_x, size_y), (sf::FloatRect)spr->getSprite().getTextureRect());
									ImGui::TreePop();
								}
								break;
							case Engine::INT:
								field = (std::string)member.getName() + ": %.i";
								ImGui::Text(field.c_str(), member.get(*ptr));
								break;
							case Engine::BOOL:
								field = (std::string)member.getName() + ": %.i";
								ImGui::Text(field.c_str(), member.get(*ptr));
								break;
							case Engine::FLOAT:
								field = (std::string)member.getName() + ": %.4f";
								ImGui::Text(field.c_str(), member.get(*ptr));
								if (ImGui::BeginPopupContextItem(field.c_str())) // Test staff
								{
									float changedValue = meta::getMemberValue<float>(*ptr, member.getName());
									if (ImGui::Selectable("Set to zero")) changedValue = 0.0f;
									if (ImGui::Selectable("Set to default")) changedValue = 0.005;
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

					if (!isFirstDraw)
					{
						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();
					}
				}
				ImGui::End();
			}
		}
	private:
		template<class Type>
		valueType checkType()
		{
			if (typeid(Type) == typeid(std::string))
				return STRING;
			else if (typeid(Type) == typeid(int))
				return INT;
			else if (typeid(Type) == typeid(bool))
				return BOOL;
			else if (typeid(Type) == typeid(float))
				return FLOAT;
			else if (typeid(Type) == typeid(sf::Vector2f))
				return VECTOR2F;
			else if (typeid(Type) == typeid(sf::Sprite))
				return SPRITE;
		}
		T* ptr;
		sf::Vector2f size = { 400,400 };
	};
	class Object
	{
	protected:
		ObjectType type;
		sf::Vector2f position;
		sf::Texture texture;
		sf::Sprite sprite;
		std::string name;
		bool IsActive;
		DebugWindows<Object> dw_o;
	public:
		Object()
		{
			type = ObjectType::None;
			dw_o.set(this);
		}
		Object(std::string);
		~Object() = default;
		Object(sf::Vector2f pos, std::string name);
		Object(sf::Vector2f pos, ObjectType t, std::string name);
		bool isActive() const { return IsActive; }
		void destroy() { IsActive = false; }
		const std::string& getName() const { return name; }
		const sf::Sprite& getSprite() { return sprite; }
		sf::Texture& getTexture() { return texture; }
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
			member("name", &Engine::Object::getName),
			member("type", &Engine::Object::type),
			member("sprite", &Engine::Object::sprite),
			member("position", &Engine::Object::position),
			member("IsActive", &Engine::Object::IsActive)
		);
	}
}
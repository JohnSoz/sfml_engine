#pragma once
#include <MetaStuff/Meta.h>
#include "imgui.h"
#include "imgui-SFML.h"
#include "staticVariable.h"
#include "LogConsole.h"
namespace Engine
{
	namespace debug
	{
		enum valueType { INT, BOOL, FLOAT, STRING, VECTOR2F, SPRITE, ANIMATIONMANAGER, RECT_i, RECT_F };
		template<class Type>
		valueType checkType()
		{
			if (std::is_same<Type, std::string>::value)
				return STRING;
			else if (std::is_same<Type, bool>::value)
				return BOOL;
			else if (std::is_same<Type, sf::Vector2f>::value)
				return VECTOR2F;
			else if (std::is_same<Type, sf::IntRect>::value)
				return RECT_i;
			else if (std::is_same<Type, sf::FloatRect>::value)
				return RECT_F;
			else if (std::is_same<Type, sf::Sprite>::value)
				return SPRITE;
			else if (std::is_integral<Type>::value)
				return INT;
			else if (std::is_floating_point<Type>::value)
				return FLOAT;
			else if (std::is_enum<Type>::value)
				return INT;
			else
				Console::AppLog::addLog("UB", Console::logType::error);
		}

		namespace utility
		{
			template<class type>
			void helper(std::string& selected)
			{
				std::string field;
				std::string name = typeid(type).name();
				ImGui::BeginGroup();
				if (ImGui::Selectable(name.c_str(), selected == name))
				{
					selected = name;
				}
				ImGui::SameLine();
				ImGui::EndGroup();
			}
			template<class Ptr, class type>
			void draw(Ptr* ptr, std::string& selected)
			{
				std::string field;
				if (typeid(type).name() == selected)
				{
					ImGui::Spacing();
					ImGui::TextColored(ImVec4(.60f, .35f, .50f, 1.f), std::string(selected).c_str());
					ImGui::Separator();
					auto ptr_type = (type*)ptr;
					meta::doForAllMembers<type>(
						[&](const auto& member)
						{if (member.canGetConstRef())
					{
						switch (checkType<decltype(member.getCopy(*ptr_type))>())
						{
						case Engine::debug::INT:
							field = (std::string)member.getName() + ": %.i";
							ImGui::Text(field.c_str(), member.get(*ptr_type));
							break;
						case Engine::debug::BOOL:
							field = (std::string)member.getName() + ": %i";
							ImGui::Text(field.c_str(), member.get(*ptr_type));
							break;
						case Engine::debug::FLOAT:
							field = (std::string)member.getName() + ": %f";
							ImGui::Text(field.c_str(), member.get(*ptr_type));
							if (ImGui::BeginPopupContextItem(field.c_str()))
							{
								float changedValue = meta::getMemberValue<float>(*ptr_type, member.getName());
								if (ImGui::Selectable("Set to zero")) changedValue = 0.0f;
								ImGui::PushItemWidth(200);
								std::string name = "#" + (std::string)member.getName();
								ImGui::DragFloat(name.c_str(), &changedValue, 0.001f, -1.f, 1.f);
								meta::setMemberValue<float>(*ptr_type, member.getName(), changedValue);
								ImGui::PopItemWidth();
								ImGui::EndPopup();
							}
							break;
						case Engine::debug::STRING:
							field = (std::string)member.getName() + ": " + meta::getMemberValue<std::string>(*ptr_type, member.getName());
							ImGui::Text(field.c_str());
							break;
						case Engine::debug::SPRITE:
							if (ImGui::TreeNode("Object Sprite"))
							{
								auto spr = (Object*)ptr;
								int size_x = spr->getSprite().getTextureRect().width;
								int size_y = spr->getSprite().getTextureRect().height;
								ImGui::Text("%.0fx%.0f", size_x, size_y);
								ImGui::Image(*spr->getSprite().getTexture(), sf::Vector2f(size_x, size_y), static_cast<sf::FloatRect>(spr->getSprite().getTextureRect()));
								ImGui::TreePop();
							}
							break;
						case Engine::debug::ANIMATIONMANAGER:
							break;
						case Engine::debug::RECT_i:
						{
							field = (std::string)member.getName() + ": (%i, %i, %i, %i)";
							auto rect = meta::getMemberValue<sf::IntRect>(*ptr_type, member.getName());
							ImGui::Text(field.c_str(), rect.left, rect.top, rect.width, rect.height);
						}
						break;
						case Engine::debug::RECT_F:
						{
							field = (std::string)member.getName() + ": (%.2f, %.2f, %.2f, %.2f)";
							auto rect_f = meta::getMemberValue<sf::FloatRect>(*ptr_type, member.getName());
							ImGui::Text(field.c_str(), rect_f.left, rect_f.top, rect_f.width, rect_f.height);
						}
						break;
						case Engine::debug::VECTOR2F:
						{
							field = (std::string)member.getName() + ": (%.2f %.2f)";
							auto vec = meta::getMemberValue<sf::Vector2f>(*ptr_type, member.getName());
							ImGui::Text(field.c_str(), vec.x, vec.y);
							if (ImGui::BeginPopupContextItem(field.c_str()))
							{
								auto changedValue = meta::getMemberValue<sf::Vector2f>(*ptr_type, member.getName());
								ImGui::PushItemWidth(200);
								ImGui::DragFloat("#X", &changedValue.x, 1.f, -999.f, 999.f);
								ImGui::DragFloat("#Y", &changedValue.y, 1.f, -999.f, 999.f);
								meta::setMemberValue<sf::Vector2f>(*ptr_type, member.getName(), changedValue);
								ImGui::PopItemWidth();
								ImGui::EndPopup();
							}
						}
						break;
						default:
							break;
						}
					}});

				}
			}
		}

		template<class PtrType, class... Types>
		void debugDraw(PtrType* ptr, std::string nameTree)
		{
			if (Engine::VStaticContainer::ShowDebugWindow)
			{
				ImGui::SetNextWindowSize(ImVec2(600, 440), ImGuiCond_FirstUseEver);
				if (ImGui::Begin("Debug Window", NULL))
				{
					static std::string selected = "";
					static bool is_prev_treeNode_open = false;

					std::string curr_selected = "";
					ImGui::BeginChild("Objects", ImVec2(300, 0), true);
					if (ImGui::TreeNode((void*)ptr, nameTree.c_str()))
					{
						(utility::helper<Types>(selected), ...);
						utility::helper<PtrType>(selected);
						curr_selected = selected;
						is_prev_treeNode_open = true;
						ImGui::TreePop();
					}
					else
						if (!is_prev_treeNode_open)
							selected = "";

					ImGui::EndChild();
					ImGui::SameLine();

					ImGui::BeginGroup();
					ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
					(utility::draw<PtrType, Types>(ptr, curr_selected), ...);
					utility::draw<PtrType, PtrType>(ptr, curr_selected);
					ImGui::EndChild();
					ImGui::SameLine();
					ImGui::EndGroup();
				}
				ImGui::End();
			}
		}
	}
}
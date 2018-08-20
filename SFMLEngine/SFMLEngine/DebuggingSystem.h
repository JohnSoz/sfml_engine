#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "imgui.h"
#include "imgui-sfml.h"
#define Debug 1;

namespace Engine
{
	struct ImGUI
	{

		static void ShowHelpMarker(const char* desc)
		{
			ImGui::TextDisabled("(?)");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(desc);
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}

		static void SimpleOverlay(bool *open)
		{
			if (*open)
			{
				const float DISTANCE = 10.0f;
				static int corner = 1;

				ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
				ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
				if (corner != -1)
					ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
				ImGui::SetNextWindowBgAlpha(0.3f);

				if (ImGui::Begin("Overlay", open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
				{
					ImGui::Text("Show player position");
					ImGui::Separator();
					if (ImGui::IsMousePosValid())
					{
						ImGui::Text("Window size: (%.1f,%.1f)", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
						//ImGui::Text("Framerate: (%.1f)", ImGui::GetIO().Framerate);
						ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
						static float values[90] = { 0 };
						static int values_offset = 0;
						static float refresh_time = 0.0f;
						//if (!animate || refresh_time == 0.0f)
						//	refresh_time = ImGui::GetTime();
						while (refresh_time < ImGui::GetTime()) // Create dummy data at fixed 60 hz rate for the demo
						{
							values[values_offset] = ImGui::GetIO().Framerate;
							values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
							refresh_time += 1.0f / 60.0f;
						}
						ImGui::PlotLines("Framerate", values, IM_ARRAYSIZE(values), values_offset, "avg 200 - 400", 200, 400, ImVec2(0, 80));
					}
					else
						ImGui::Text("Player Position: <invalid>");
					if (ImGui::BeginPopupContextWindow())
					{
						if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
						if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
						if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
						if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
						if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
						if (*open && ImGui::MenuItem("Close")) *open = false;
						ImGui::EndPopup();
					}
					ImGui::End();
				}
			}
		}

		static void SimpleText(sf::Vector2f position, bool *open, std::string name)
		{
			if (*open)
			{
				ImVec2 window_pos = ImVec2(position.x, position.y);
				ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(0, 0));
				ImGui::SetNextWindowBgAlpha(0.6f);
				ImGui::SetNextWindowSize(ImVec2(102, 70));
				if (ImGui::Begin(name.c_str(), open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
				{
					ImGui::SetWindowFontScale(1);
					ImGui::Text("Info:");
					ImGui::Separator();
					ImGui::Text("PosX: (%.1f)", position.x);
					ImGui::Text("PosY: (%.1f)", position.y);
					ImGui::End();
				}
			}
			else
				return;
		}

		static void objectStatusInfo(bool *open, std::string name, std::string info)
		{
			if (ImGui::Begin("Demo window", open))
			{
				ImGui::SetWindowSize(ImVec2(300, 300));
				if (ImGui::CollapsingHeader("Game info"))
				{
					if (ImGui::TreeNode("Object info"))
					{
						ImGui::Text("Entity:");
						ImGui::Separator();
						ImGui::Text(info.c_str());
						static float value = 0.5f;
						ImGui::Text(" Value = %.3f (<-- right-click here)", value);
						if (ImGui::BeginPopupContextItem("item context menu"))
						{
							if (ImGui::Selectable("Set to zero")) value = 0.0f;
							if (ImGui::Selectable("Set to PI")) value = 3.1415f;
							ImGui::PushItemWidth(-1);
							ImGui::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
							ImGui::PopItemWidth();
							ImGui::EndPopup();
						}
						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Change object"))
					{
						ImGui::Text("Entity:");
						static char str0[128] = "Hello, world!";
						static int i0 = 123;
						ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));
						ImGui::InputInt("input int", &i0);
						ImGui::TreePop();
					}
				}
				ImGui::End();
			}
		}

	};

	class DebuggingSystem
	{
	public:
		static sf::RenderWindow* window;
		std::vector<std::pair<Rectangle*, Rectangle*>> entites;
		std::vector<sf::FloatRect> obj;
		std::vector<Entity*> entite;
		bool overlay = true;
		sf::Clock Pressclock;
	public:
		static void setWindow(sf::RenderWindow& w)
		{
			window = &w;
		}
		void draw();
		void pushEntites(Entity& e)
		{
			entite.push_back(&e);
		}
		void pushRectangle(std::pair<Rectangle*, Rectangle*> e)
		{
			entites.push_back(e);
		}
		void levelObjects(std::vector<ObjectLevel> objs)
		{
			for (auto i : objs)
				obj.push_back(i.rect);
		}
		void updateDebugInfo(std::string i)
		{
			//ImGui::ShowDemoWindow();
			ImGUI::objectStatusInfo(&overlay, "asd", i);
		}
		void handleEvent(sf::Event& event);
		DebuggingSystem() = default;
		~DebuggingSystem() = default;
	};
}


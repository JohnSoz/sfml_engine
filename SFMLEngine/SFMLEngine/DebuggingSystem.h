#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "imgui.h"
#include "imgui-sfml.h"
#include <array>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#define Debug 1;

using namespace std;

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
		enum logType {
			error = 1,
			info,
			fatal,
			system
		};

		struct Log
		{
			Log(std::string s, ImVec4 c, logType t)
			{
				type = t;
				color = c;
				boost::posix_time::ptime utcCur = boost::posix_time::second_clock::local_time();
				std::string l = "[sec:" + std::to_string(utcCur.time_of_day().seconds());
				switch (t)
				{
				case Engine::ImGUI::error:
					l += "type:error]: ";
					break;
				case Engine::ImGUI::info:
					l += "type:info]: ";
					break;
				case Engine::ImGUI::fatal:
					l += "type:fatal]: ";
					break;
				case Engine::ImGUI::system:
					l += "type:system]: ";
					break;
				}
				l += s;
				l += " \n";
				text = l;
			}

			std::string              text;
			ImVec4                   color;
			logType                  type;
		};

		struct AppLog
		{
			ImVector<int>            LineOffsets;        // Index to lines offset
			bool                     ScrollToBottom;
			std::vector<Log>         Buffer;

			void Clear() { Buffer.clear(); Buffer.shrink_to_fit(); }

			void addLog(Log log)
			{
				Buffer.push_back(log);
			}

			void Draw(const char* title, bool *p_open)
			{
				if (*p_open)
				{
					//ImVec2 window_pos = ImVec2(0, 0);
					ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always, ImVec2(0, 0));
					ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y * 0.2));
					ImGui::SetNextWindowBgAlpha(0.8f);
					ImGui::Begin(title, p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
					if (ImGui::Button("Clear")) Clear();
					ImGui::SameLine();
					bool copy = ImGui::Button("Copy");
					ImGui::SameLine();
					static bool find = false;
					ImGui::Checkbox("Find By Type", &find);

					const char* items[] = { "all","error", "info", "fatal", "system" };
					static const char* item_current = items[0];
					ImGui::PushItemWidth(45);
					if (ImGui::BeginCombo("", item_current, ImGuiComboFlags_NoArrowButton))
					{

						for (int n = 0; n < IM_ARRAYSIZE(items); n++)
						{
							bool is_selected = (item_current == items[n]);
							if (ImGui::Selectable(items[n], is_selected))
								item_current = items[n];
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
					ImGui::PopItemWidth();
					ImGui::SameLine();
					std::string input;
					char buff[256] = { 0 };
					ImGui::PushItemWidth(280);
					bool value_changed = ImGui::InputText("Filter", buff, IM_ARRAYSIZE(buff));
					ImGui::PopItemWidth();
					input = buff;
					ImGui::Separator();

					ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
					if (copy) ImGui::LogToClipboard();

					if (value_changed)
					{
						for (auto item : Buffer)
						{
							std::string found = "";
							std::size_t start_pos = -1;
							start_pos = item.text.find_first_of(" ");
							for (int i = 0; i < input.size(); i++)
							{
								std::size_t pos_begin = -1;

								pos_begin = item.text.find_first_of(input[i], start_pos);
								if (pos_begin != -1)
								{
									found += item.text.substr(pos_begin, 1);
								}
							}
							if (found == input)
							{
								ImGui::TextColored(ImVec4(140, 50, 60, 255), item.text.c_str()); //Test
							}
						}

					}
					else if (find && item_current != "all")
					{
						for (auto item : Buffer)
						{
							if (item_current == "error")
							{
								if (item.type == logType::error)
									ImGui::TextColored(ImVec4(0, 0, 100, 255), item.text.c_str());
							}
							else if (item_current == "info")
							{
								if (item.type == logType::info)
									ImGui::TextColored(ImVec4(0, 0, 100, 255), item.text.c_str());
							}
							else if (item_current == "fatal")
							{
								if (item.type == logType::fatal)
									ImGui::TextColored(ImVec4(0, 0, 100, 255), item.text.c_str());
							}
							else if (item_current == "system")
							{
								if (item.type == logType::system)
									ImGui::TextColored(ImVec4(0, 0, 100, 255), item.text.c_str());
							}

						}
					}
					else
					{
						for (auto i : Buffer)
							ImGui::TextColored(i.color, i.text.c_str()); //Test
					}

					if (ScrollToBottom)
						ImGui::SetScrollHere(1.0f);
					ScrollToBottom = false;
					ImGui::EndChild();
					ImGui::End();
				}
			}
		};
		/*static void objectStatusInfo(bool *open, std::string name, Actor& obj)
		{
			if (ImGui::Begin("Demo window", open))
			{
				ImGui::SetWindowSize(ImVec2(300, 300));

				auto[isWalk, life, isCollision, energy, velocityx, velocityy] = obj.getDebugData();

				if (ImGui::CollapsingHeader("Game info"))
				{
					if (ImGui::TreeNode("Object info"))
					{
						ImGui::Text("Entity:");
						ImGui::Separator();
						ImGui::Text("Value %.3f", *energy);
						if (ImGui::BeginPopupContextItem("item context menu"))
						{
							if (ImGui::Selectable("Set to zero")) *energy = 0.0f;
							if (ImGui::Selectable("Set to default")) *energy = 0.005;
							ImGui::PushItemWidth(200);
							ImGui::DragFloat("#Value", energy, 0.001f, 0.001f, 0.09f);
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
		}*/

	};

	class DebuggingSystem
	{
	public:
		static sf::RenderWindow* window;
		std::vector<std::pair<Rectangle*, Rectangle*>> entites;
		std::vector<sf::FloatRect> obj;
		bool overlay = true;
		bool LogConsole = false;
		sf::Clock Pressclock;
		static ImGUI::AppLog *log;
	public:
		static void setWindow(sf::RenderWindow& w)
		{
			window = &w;
		}
		void draw();
		void pushRectangle(std::pair<Rectangle*, Rectangle*> e)
		{
			entites.push_back(e);
		}
		void levelObjects(std::vector<ObjectLevel> objs)
		{
			for (auto i : objs)
				obj.push_back(i.rect);
		}
		void handleEvent(sf::Event& event);
		DebuggingSystem() = default;
		~DebuggingSystem() = default;
	};
}


#pragma once
#include "Object.h"
#include "imgui.h"
#include "imgui-sfml.h"

#define Debug 1;

using namespace std;

namespace Engine
{
	struct ImGUI
	{

		static void ShowHelpMarker(const char* desc);

		static void SimpleOverlay(bool *open);

		static void SimpleText(sf::Vector2f position, bool *open, std::string name);

		enum logType {
			error = 1,
			info,
			fatal,
			system
		};

		struct Log
		{
			Log(std::string s, ImVec4 c, logType t);

			std::string              text;
			ImVec4                   color;
			logType                  type;
		};

		struct AppLog
		{
			bool                     ScrollToBottom;
			std::vector<Log>         Buffer;
			const char* items[5] = { "all","error", "info", "fatal", "system" };
			void Clear() { Buffer.clear(); Buffer.shrink_to_fit(); }

			void addLog(Log log);

			void Draw(const char* title, bool *p_open);
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
		std::vector<std::pair<Engine::Rectangle*, Engine::Rectangle*>> entites;
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
		void pushRectangle(std::pair<Engine::Rectangle*, Engine::Rectangle*> e)
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


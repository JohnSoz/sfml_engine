#pragma once
#include "Object.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "DebuggingSystem.h"

#if Debug 
#include "imgui.h"
#include "imgui-sfml.h"
#endif

namespace Engine
{
	enum appState
	{
		Pause = 0,
		Run,
		Stop,
		Err
	};

	class ObjectHandler
	{
	private:
		std::vector<Entity*> ObjectsArray;
		std::vector<Entity*>::iterator iter;
#if Debug
		Debugging::DebuggingSystem debug;
#endif
	public:
		ObjectHandler() = default;
		~ObjectHandler() = default;

		template<class Obj>
		Obj& GetObjects(std::string NAME);
		void PushObject(Entity* obj);
		void UpdateObjects(float time);
		void RenderObjects(sf::RenderWindow &WINDOW);
		void refresh()
		{
			ObjectsArray.erase(std::remove_if(std::begin(ObjectsArray), std::end(ObjectsArray), [](const Entity *entity)
			{
				return !entity->isActive();
			}));
		}
	};

	class World
	{
	private:
		static float time;
		sf::Clock mainClock;
		sf::RenderWindow* window;
		ObjectHandler objHandler;
		appState state;
#if Debug 
		bool ShowOverlay = true;
#endif
		void update();
		void handleEvent(sf::Event& event);
		void draw();
		void init();

	public:
		World(sf::RenderWindow& w) : window(&w) {};
		~World() { delete window; };

		void startGame();
		ObjectHandler& getObjHendler() { return objHandler; }
		static float getTime() { return time; }
	};

#if Debug
	struct  ImGUI
	{
		static void SimpleOverlay(sf::Vector2f pos, bool *open)
		{
			if (*open)
			{
				const float DISTANCE = 10.0f;
				static int corner = 0;

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
						ImGui::Text("Framerate: (%.1f)", ImGui::GetIO().Framerate);
						ImGui::Text("Player Position: (%.1f,%.1f)", pos.x, pos.y);
						ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
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
	};
#endif
}

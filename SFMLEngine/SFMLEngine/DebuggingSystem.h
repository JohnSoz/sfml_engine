#pragma once
/*!
\file
\brief Header file.
\warning Dependency on Object.h
*/

#include "Object.h"
#include "imgui.h"
#include "imgui-sfml.h"
#include "timer.h"
#include "Level.h"
#include "Math.h"
#include <vector>
using namespace std;

namespace Engine
{
	namespace ImGUI
	{
		void ShowHelpMarker(const char* desc);
		void SimpleOverlay(bool* open);
		void SimpleText(sf::Vector2f position, bool* open, std::string name, std::string text = "");
		void Text(sf::Vector2f position, bool* open, std::string name, std::string text = "");
		//void windowTest(bool* p_open);
	} // namespace ImGUI

	class DebuggingSystem
	{
	public:
		static sf::RenderWindow* window;
		std::vector<std::pair<const sf::FloatRect*, const sf::FloatRect*>> entites;
		std::vector<std::pair<std::string, sf::FloatRect>> obj;
		std::vector<std::pair<std::string, Quad>> iso_obj;
		bool overlay = false;
		bool showVertex = false;
		sf::Clock Pressclock;
		void drawDebugWindows(bool open)
		{
			if (open)
			{
				ImGui::Begin("Debug Window", NULL);
				ImGui::End();
			}
		}

	public:
		DebuggingSystem() = default;
		~DebuggingSystem() = default;

		static void setWindow(sf::RenderWindow& w) { window = &w; }
		void draw(sf::RenderTarget& target = *window);
		void pushRectangle(const std::pair<const sf::FloatRect*, const sf::FloatRect*> e)
		{
			entites.push_back(e);
		}
		void levelObjects(const std::vector<ObjectLevel>& objs)
		{
			for (auto const& i : objs)
				obj.emplace_back(i.name, i.rect);
		}
		void levelIsoObjects(const std::vector<ObjectLevel>& objs)
		{
			for (auto const& i : objs)
				iso_obj.emplace_back(i.name, i.quad);
		}
		void handleEvent(sf::Event& event);
	};
} // namespace Engine

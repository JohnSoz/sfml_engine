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
#include <vector>
#define _Debug_ 1; ///<warning The global flag. Debug = 1 when you need to debug
using namespace std;

namespace Engine
{
	struct ImGUI
	{
		static void ShowHelpMarker(const char* desc);
		static void SimpleOverlay(bool *open);
		static void SimpleText(sf::Vector2f position, bool *open, std::string name);
	};

	class DebuggingSystem
	{
	public:
		static sf::RenderWindow* window;
		std::vector<std::pair<sf::FloatRect*, sf::FloatRect*>> entites;
		std::vector<sf::FloatRect> obj;
		bool overlay = false;
		bool LogConsole = false;
		bool showVertex = false;
		sf::Clock Pressclock;
	public:
		DebuggingSystem() = default;
		~DebuggingSystem() = default;

		static void setWindow(sf::RenderWindow& w) { window = &w; }
		void draw();
		void pushRectangle(std::pair<sf::FloatRect*, sf::FloatRect*> e)
		{
			entites.push_back(e);
		}
		void levelObjects(std::vector<ObjectLevel> objs)
		{
			for (auto i : objs)
				obj.push_back(i.rect);
		}
		void handleEvent(sf::Event& event);
	};
}


#pragma once
#include "Gui.h"

namespace Engine
{
	class MainMenu //: public GUI
	{
	private:
		GroupArray groupArray;
		Gui gui;

	public:
		MainMenu() = default;
		MainMenu(std::string pathToTheme, sf::RenderWindow& w) : groupArray(w, pathToTheme)
		{
			gui.setTarget(w);
			sf::Font font;
			font.loadFromFile("Data/Fonts/Bricks.otf");
			gui.setFont(font);
			gui.add(groupArray[0]);
		}
		void makeMenu(std::string&);
		void draw()
		{
			gui.draw();
		}
		void update() {}
		void handleEvent(sf::Event& e)
		{
			gui.handleEvent(e);
		}
	};
}
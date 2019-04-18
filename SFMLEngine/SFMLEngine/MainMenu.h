#pragma once
#include "Gui.h"
#include "GuiEditor.h"
#include "staticVariable.h"

namespace Engine
{
	class MainMenu final : public BaseGui
	{
	private:
		GuiEditor gEditor;
	public:
		MainMenu(std::string pathToTheme, sf::RenderWindow& w) : BaseGui(w, pathToTheme), gEditor(*this)
		{
			gui.setTarget(w);
			sf::Font font;
			font.loadFromFile("Data/Fonts/Bricks.otf");
			gui.setFont(font);
			gui.add(groupArray[0]);
		}
		void makeMenu(std::string&);
		void draw() override
		{
			if (VStaticContainer::ShowGuiEditor)
				gEditor.drawEditor();
			gui.draw();
		}
		void update() {}
		void handleEvent(sf::Event& e)override
		{
			gui.handleEvent(e);
		}
	};
}
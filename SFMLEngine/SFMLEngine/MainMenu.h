#pragma once
#include "Gui.h"
#include "GuiEditor.h"

namespace Engine
{
	class MainMenu final : public BaseGui
	{
	private:
		GuiEditor gEditor;
	public:
		MainMenu(std::string pathToTheme, sf::RenderWindow& w);

		void draw() override;
		void update() override {}
		void handleEvent(sf::Event& e) override;
		void makeMenu();
	};
}
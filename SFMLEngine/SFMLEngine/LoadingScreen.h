#pragma once
#include "Gui.h"
#include "GuiEditor.h"
namespace Engine
{
	class LoadingScreen final : public BaseGui
	{
	private:
		GuiEditor gEditor;
		tgui::TextBox::Ptr text;
		sf::Clock time;
	public:
		LoadingScreen(std::string pathToTheme, sf::RenderWindow& w);

		void draw() override;
		void update() override 
		{
			std::string time_str = "  LoadingTime: " + std::to_string(time.getElapsedTime().asSeconds());
			text->setText(time_str);
		}

		void handleEvent(sf::Event& e) override;
		void makeMenu();
	};
}


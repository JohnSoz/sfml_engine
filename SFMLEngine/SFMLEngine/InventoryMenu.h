#pragma once
#include "GUI.h"
#include <entityx/entityx.h>
#include "GuiEditor.h"
#include "Inventory.h"
#include "EngineEvents.h"

namespace Engine
{
	class InventoryMenu : public BaseGui, public entityx::Receiver<InventoryMenu>
	{
		Gui gui;
		Inventory* inv;
	public:
		InventoryMenu() = default;
		InventoryMenu(std::string pathToTheme, sf::RenderWindow& w) : BaseGui(w, pathToTheme)
		{
			gui.setTarget(w);
			sf::Font font;
			font.loadFromFile("Data/Fonts/Bricks.otf");
			gui.setFont(font);
			gui.add(groupArray[0].second);
		}
		~InventoryMenu() = default;

		void receive(const Events::Event_Inventory_UI&)
		{
			activateOrDisable();
			update();
		}

		void makeMenu(Inventory& inventr);

		void update() override;

		void moveAll(sf::Vector2f offset);

		void draw() override
		{
			//g.drawEditor();
			gui.draw();
		}

		void handleEvent(sf::Event& e) override
		{
			gui.handleEvent(e);
		}
	};
}
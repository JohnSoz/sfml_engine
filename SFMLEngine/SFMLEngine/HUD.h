#pragma once
#include "GUI.h"
#include "Inventory.h"
#include "LogConsole.h"

#include "EngineEvents.h"
#include "GuiEditor.h"
namespace Engine
{
	class HUD : public BaseGui
	{
		Inventory* inv;
		GuiEditor gEditor;
	public:
		HUD() {}
		HUD(std::string pathToTheme, sf::RenderWindow& w, Inventory& i) : BaseGui(w, pathToTheme), gEditor(*this)
		{
			inv = &i;
			gui.setTarget(w);
			sf::Font font;
			font.loadFromFile("Data/Fonts/Bricks.otf");
			gui.setFont(font);
			gui.add(groupArray[0]);
		}

		void makeHud()
		{
			groupArray.addWidget(makePicture(inv->getCurrItem<Item>()->getTexture().copyToImage(), { 100.f,800.f }, 0.94f), "Item");
		}

		void update() override
		{
			auto item = groupArray[0]->get<tgui::Picture>("Item");
			item->getRenderer()->setTexture(inv->getCurrItem<Item>()->getTexture());
			item->setSize((sf::Vector2f)inv->getCurrItem<Item>()->getTexture().getSize());
		}

		void draw() override
		{
			if (VStaticContainer::ShowGuiEditor)
				gEditor.drawEditor();
			gui.draw();
		}
	};
}
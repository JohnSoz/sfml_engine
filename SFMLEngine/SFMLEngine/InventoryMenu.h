#pragma once
#include "GUI.h"
#include "Inventory.h"
#include "LogConsole.h"
#include <entityx/entityx.h>
#include "EngineEvents.h"
#include "GuiEditor.h"
using namespace entityx;

namespace Engine
{
	class InventoryMenu : public BaseGui, public Receiver<InventoryMenu>
	{
		Gui gui;
		GuiEditor g;
	public:
		InventoryMenu() = delete;
		InventoryMenu(std::string pathToTheme, sf::RenderWindow& w) : BaseGui(w, pathToTheme), g(*this)
		{
			gui.setTarget(w);
			sf::Font font;
			font.loadFromFile("Data/Fonts/Bricks.otf");
			gui.setFont(font);
			gui.add(groupArray[0]);
		}
		~InventoryMenu() = default;

		void receive(const Events::Event_Inventory_UI&)
		{
			activateOrDisable();
		}

		void makeMenu(Inventory& inv)
		{
			Console::AppLog::addLog(Console::Log("Engine::InventoryMenu::makeMenu()", Console::logType::info));
			sf::Texture t;
			t.loadFromFile("Data/images/bgTest.png");
			groupArray.addWidget(makePicture(t, { 0.f,0.f }, { 1920,1080 }, 0.94f), "BackGround");

			sf::Texture tex;
			tex.loadFromFile("Data/images/invBG.png");
			groupArray.addWidget(makePicture(tex, { 300,300 }, { 1092,548 }), "InvBG");

			int w = 0;
			int h = 0;
			tgui::Theme theme;
			theme.load("Data/GUI/MyUI/MainMenu.txt");
			std::vector<tgui::Button::Ptr> buttonArray;
			auto grid = tgui::Grid::create();

			tgui::ToolTip::setTimeToDisplay(sf::milliseconds(200));
			tgui::ToolTip::setDistanceToMouse({ 20, 20 });

			auto container_ = tgui::Group::create({ 813, 530 });
			container_->setPosition({ 590, 310 });

			for (int i = 0; i < inv.getSize(); i++)
			{
				if (w >= 6)
				{
					w = 0;
					h++;
				}
				auto inv_item = inv.getItem<Item>(i);
				auto button = tgui::BitmapButton::create();
				auto toolTip = tgui::Label::create(inv_item->getName());
				toolTip->setRenderer(theme.getRenderer("ToolTip"));
				toolTip->setTextSize(22);
				button->setUserData(inv_item->getName());
				button->setToolTip(toolTip);
				button->setImage(inv_item->getTexture());
				button->setRenderer(theme.getRenderer("ButtonInv"));
				button->setSize(128, 84);
				buttonArray.push_back(button);
				grid->addWidget(button, h, w, tgui::Padding(2));
				w++;
			}

			for (auto& w : grid->getWidgets())
			{
				w->connect("pressed", [&]()
				{
					//auto wid = w->cast<tgui::Button>();
					auto gun_name = w->getUserData<std::string>();
					inv.setItemByName(gun_name);
					std::string log = "Button " + inv.getCurrItem<Gun>()->getName();
					Console::AppLog::addLog(log, Console::logType::info);
				});
			}
			container_->add(grid);
			groupArray.addWidget(container_, "InventoryContainer");
			activateOrDisable();
		}

		void updateInventory(Inventory& inv)
		{
			auto panel = groupArray.get("Default")->get<tgui::Panel>("InventoryContainer");
		}

		void moveAll(sf::Vector2f offset)
		{
			for (auto& w : groupArray[0]->getWidgets())
				w->setPosition(w->getPosition() + offset);
		}

		void draw() override
		{
			//g.drawEditor();
			gui.draw();
		}

		void update() {}

		void handleEvent(sf::Event & e) override
		{
			gui.handleEvent(e);
		}
	};
}
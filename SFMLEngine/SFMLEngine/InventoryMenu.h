#pragma once
#include "GUI.h"
#include "Inventory.h"
#include "LogConsole.h"
#include <entityx/entityx.h>
#include "EngineEvents.h"
#include "GuiEditor.h"

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
			gui.add(groupArray[0]);
		}
		~InventoryMenu() = default;

		void receive(const Events::Event_Inventory_UI&)
		{
			activateOrDisable();
			update();
		}

		void makeMenu(Inventory& inventr)
		{
			inv = &inventr;
			//sf::Context c; //<WTF?? For some reason I have to create opengl context in this place 
			//               //Without it, any copying of the texture (outside the main thread) causes an error when calling glFlush() in sf::Texture(const Texture&)
			Console::AppLog::addLog(Console::Log("Engine::InventoryMenu::makeMenu()", Console::logType::info));
			groupArray.addWidget(makePicture("Data/images/bgTest.png", { 0.f,0.f }, { 1920,1080 }, 0.94f), "BackGround");
			groupArray.addWidget(makePicture("Data/images/invBG.png", { 300,300 }, { 1092,548 }), "InvBG");
			int w = 0;
			int h = 0;
			tgui::Theme theme;
			theme.load("Data/GUI/MyUI/MainMenu.txt");
			auto grid = tgui::Grid::create();

			tgui::ToolTip::setTimeToDisplay(sf::milliseconds(200));
			tgui::ToolTip::setDistanceToMouse({ 20, 20 });

			auto container_ = tgui::Group::create({ 813, 530 });
			container_->setPosition({ 590, 310 });
			for (auto& inv_item : *inv)
			{
				if (w >= 6)
				{
					w = 0;
					h++;
				}
				//auto inv_item = inv->getItem<Item>(i);
				auto button = tgui::BitmapButton::create();
				auto toolTip = tgui::Label::create(inv_item->getName());
				toolTip->setRenderer(theme.getRenderer("ToolTip"));
				toolTip->setTextSize(22);
				button->setUserData(inv_item->getName());
				button->setToolTip(toolTip);
				button->setImage(inv_item->getTexture());
				button->setRenderer(theme.getRenderer("ButtonInv"));
				button->setSize(128, 84);
				grid->addWidget(button, h, w, tgui::Padding(2));
				w++;
			}
			for (auto& w : grid->getWidgets())
			{
				w->connect("pressed", [&]()
					{
						auto gun_name = w->getUserData<std::string>();
						inv->setItemByName(gun_name);
						std::string log = "Button " + inv->getCurrItem<Item>()->getName();
						Console::AppLog::addLog(log, Console::logType::info);
					});
			}
			container_->add(grid);
			groupArray.addWidget(container_, "InventoryContainer");
			activateOrDisable();
		}

		void update() override
		{
			auto cont = groupArray.get("Default")->get<tgui::Group>("InventoryContainer");
			auto grid = tgui::Grid::create();
			cont->removeAllWidgets();

			int w = 0;
			int h = 0;
			for (int i = 0; i < inv->getSize(); i++)
			{
				if (w >= 6)
				{
					w = 0;
					h++;
				}
				auto inv_item = inv->getItem<Item>(i);
				auto button = tgui::BitmapButton::create();
				auto toolTip = tgui::Label::create(inv_item->getName());
				toolTip->setRenderer(groupArray.getTheme().getRenderer("ToolTip"));
				toolTip->setTextSize(22);
				button->setUserData(inv_item->getName());
				button->setToolTip(toolTip);
				button->setImage(inv_item->getTexture());
				button->setRenderer(groupArray.getTheme().getRenderer("ButtonInv"));
				button->setSize(128, 84);
				grid->addWidget(button, h, w, tgui::Padding(2));
				w++;
			}
			for (auto& w : grid->getWidgets())
			{
				w->connect("pressed", [&]()
					{
						auto gun_name = w->getUserData<std::string>();
						inv->setItemByName(gun_name);
						std::string log = "Button " + inv->getCurrItem<Item>()->getName();
						Console::AppLog::addLog(log, Console::logType::info);
					});
			}
			cont->add(grid);
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

		void handleEvent(sf::Event& e) override
		{
			gui.handleEvent(e);
		}
	};
}
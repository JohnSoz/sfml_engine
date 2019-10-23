#include "MainMenu.h"
#include "EngineEvents.h"
#include <filesystem>
#include "ApplicationState.h"
#include "staticVariable.h"

namespace fs = std::filesystem;

Engine::MainMenu::MainMenu(std::string pathToTheme, sf::RenderWindow& w)
	: BaseGui(w, pathToTheme), gEditor(*this)
{
	gui.setTarget(w);
	sf::Font font;
	font.loadFromFile("Data/Fonts/Bricks.otf");
	gui.setFont(font);
	gui.add(groupArray[0]);
}

void Engine::MainMenu::makeMenu()
{
	Console::AppLog::addLog(Console::Log("Engine::MainMenu::makeMenu()", Console::logType::info));

	groupArray.addWidget(makeCanvas("Data/images/bg.png"), "Canvas");

	gui.add(groupArray.addGroup("Settings"));
	groupArray.addWidget(makePicture("Data/images/Keyboard Buttons.png", sf::Vector2f(0, 0), sf::Vector2f(1920, 1080)), "SettingsPic", "Settings");
	activateOrDisable("Settings");

	gui.add(groupArray.addGroup("ResumeMenu"));
	groupArray.addWidget(makeListBox({}, sf::Vector2f(400, 600), sf::Vector2f(250, 150),
		[this]()
		{
			std::string p = "Data/Level/";
			p += groupArray[0]->get<tgui::ListBox>("TestListBox2")->getSelectedItem();
			Console::AppLog::addLog(p, Console::info);
		}), "TestListBox2", "ResumeMenu");
	tgui::ListBox::Ptr listBox = groupArray[2]->get<tgui::ListBox>("TestListBox2");
	std::string path = "Data\\";
	for (auto& p : fs::directory_iterator(path))
	{
		std::string fileName = p.path().filename().generic_string();
		int pos = -1;
		pos = fileName.find_last_of(".");
		if (pos != -1) {
			auto fileExtension = fileName.substr(pos+1, fileName.size());
			if (pos > 0 && pos < 100 && fileExtension == "json")
			{
				fileName.erase(pos, fileName.size());
				listBox->addItem(fileName);
			}
		}
	}
	activateOrDisable("ResumeMenu");

	groupArray.addWidget(makeButton("Resume", sf::Vector2f(380, 230), sf::Vector2f(220, 100),
		[&]()
		{
			disableAllExcept("ResumeMenu");
			//EventManager::eventManager.emit<Events::Change_State_Event>(appState::Play, true, actions::loading);
		}
	), "LoadButton");

	groupArray.addWidget(makeButton("Play", sf::Vector2f(380, 360), sf::Vector2f(220, 100),
		[&]()
		{
			EventManager::eventManager.emit<Events::Change_State_Event>(appState::Play, true);
		}
	), "RunButton");

	groupArray.addWidget(makeButton("Settings", sf::Vector2f(380, 490), sf::Vector2f(220, 100),
		[&]()
		{
			disableAllExcept("Settings");
		}
	), "SettingsButton");

	groupArray.addWidget(makeButton("Exit", sf::Vector2f(380, 620), sf::Vector2f(220, 100),
		[&]()
		{
			EventManager::eventManager.emit<Events::Change_State_Event>(appState::Exit);
		}
	), "ExitButton");

	groupArray.addWidget(makeTextBox(" *GameName*", sf::Vector2f(256, 80), sf::Vector2f(960, 80)), "RandomBox");
}

void Engine::MainMenu::draw()
{
	if (VStaticContainer::ShowGuiEditor)
		gEditor.drawEditor();
	gui.draw();
}

void Engine::MainMenu::handleEvent(sf::Event& e)
{
	gui.handleEvent(e);
}

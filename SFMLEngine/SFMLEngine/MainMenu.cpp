#include "MainMenu.h"
#include "EngineEvents.h"
#include <filesystem>
#include "ApplicationState.h"

namespace fs = std::experimental::filesystem;

Engine::MainMenu::MainMenu(std::string pathToTheme, sf::RenderWindow& w)
	: BaseGui(w, pathToTheme), gEditor(*this)
{
	gui.setTarget(w);
	sf::Font font;
	font.loadFromFile("Data/Fonts/Bricks.otf");
	gui.setFont(font);
	gui.add(groupArray[0]);
}

void Engine::MainMenu::makeMenu(std::string& p)
{
	Console::AppLog::addLog(Console::Log("Engine::MainMenu::makeMenu()", Console::logType::info));

	groupArray.addWidget(makeCanvas("Data/images/bg.png"), "Canvas");

	//gui.add(groupArray.addGroup("Settings"));
	//groupArray.addWidget(makePicture("Data/images/Keyboard Buttons.png", sf::Vector2f(0, 0), sf::Vector2f(1920, 1080)), "SettingsPic", "Settings");

	//activateOrDisable("Settings");

	groupArray.addWidget(makeButton("Resume", sf::Vector2f(380, 230), sf::Vector2f(220, 100),
		[&]()
		{
			EventManager::eventManager.emit<Events::Main_Menu_Event>(appState::Loading);
		}
	), "LoadButton");

	groupArray.addWidget(makeButton("Play", sf::Vector2f(380, 360), sf::Vector2f(220, 100),
		[&]()
		{
			EventManager::eventManager.emit<Events::Main_Menu_Event>(appState::StartGame);
		}
	), "RunButton");

	groupArray.addWidget(makeButton("Settings", sf::Vector2f(380, 490), sf::Vector2f(220, 100),
		[&]()
		{
			//activateOrDisable("Default");
			//activateOrDisable("Settings");
		}
	), "SettingsButton");

	groupArray.addWidget(makeButton("Exit", sf::Vector2f(380, 620), sf::Vector2f(220, 100),
		[&]()
		{
			EventManager::eventManager.emit<Events::Main_Menu_Event>(appState::Exits);
		}
	), "ExitButton");

	/*groupArray.addWidget(makeListBox({}, sf::Vector2f(400, 600), sf::Vector2f(250, 150),
		[&]()
	{
		p = "Data/Level/";
		p += groupArray[0]->get<tgui::ListBox>("TestListBox2")->getSelectedItem();
		Console::AppLog::addLog(p, Console::info);
	}), "TestListBox2");

	tgui::ListBox::Ptr listBox = groupArray[0]->get<tgui::ListBox>("TestListBox2");
	std::string path = "Data\\Level";
	for (auto& p : fs::directory_iterator(path))
	{
		std::string fileName = p.path().filename().generic_string();
		const size_t pos = fileName.find_last_of(".");
		if (pos > 0 && pos < 100)
		{
			fileName.erase(pos, 4);
			listBox->addItem(fileName);
		}
	}*/
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

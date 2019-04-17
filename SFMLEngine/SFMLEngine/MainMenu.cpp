#include "MainMenu.h"
#include "EngineEvents.h"
#include <filesystem>

namespace fs = std::experimental::filesystem;

void Engine::MainMenu::makeMenu(std::string& p)
{
	Console::AppLog::addLog(Console::Log("Engine::MainMenu::makeMenu()", Console::logType::info));
	sf::Texture t;
	t.loadFromFile("Data/images/bg.png");
	sf::Sprite spr(t);

	groupArray.addWidget(makeCanvas(spr),"Canvas");

	groupArray.addWidget(makeButton("Play", sf::Vector2f(220, 250), sf::Vector2f(150, 60),
		[&]()
	{
		EventManager::eventManager.emit<Events::Main_Menu_Event>(appState::StartGame);
	}
	), "RunButton");

	groupArray.addWidget(makeButton("Exit", sf::Vector2f(380, 250), sf::Vector2f(150, 60),
		[&]()
	{
		EventManager::eventManager.emit<Events::Main_Menu_Event>(appState::Exits);
	}
	), "ExitButton");

	groupArray.addWidget(makeListBox({}, sf::Vector2f(400, 600), sf::Vector2f(250, 150),
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
	}
	groupArray.addWidget(makeTextBox("TestMenu", sf::Vector2f(120, 60), sf::Vector2f(960, 40)), "RandomBox");
}
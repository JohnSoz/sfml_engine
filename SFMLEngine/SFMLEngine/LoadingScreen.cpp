#include "LoadingScreen.h"
#include "staticVariable.h"
#include "EngineEvents.h"
#include <filesystem>
#include "ApplicationState.h"

Engine::LoadingScreen::LoadingScreen(std::string pathToTheme, sf::RenderWindow& w)
	: BaseGui(w, pathToTheme), gEditor(*this)
{
	gui.setTarget(w);
	sf::Font font;
	font.loadFromFile("Data/Fonts/Bricks.otf");
	gui.setFont(font);
	gui.add(groupArray[0]);
}
void Engine::LoadingScreen::draw()
{
	if (VStaticContainer::ShowGuiEditor)
		gEditor.drawEditor();
	gui.draw();
}

void Engine::LoadingScreen::handleEvent(sf::Event& e)
{
	gui.handleEvent(e);
}

void Engine::LoadingScreen::makeMenu()
{
	Console::AppLog::addLog(Console::Log("Engine::LoadingScreen::makeMenu()", Console::logType::info));
	groupArray.addWidget(makeCanvas("Data/images/LoadingScreen.png"), "Canvas");
	groupArray.addWidget(makeTextBox(".", sf::Vector2f(190, 80), sf::Vector2f(100, 100), 16), "TimeBox");
	text = groupArray[0]->get<tgui::TextBox>("TimeBox");
}

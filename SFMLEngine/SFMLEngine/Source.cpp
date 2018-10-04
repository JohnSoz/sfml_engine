#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "DebuggingSystem.h"
#include <cmath>
#include "LogConsole.h"
using namespace std;
using namespace luabridge;

//#main
int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 16;
	settings.majorVersion = 3;
	settings.minorVersion = 0;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Game V0.00001", sf::Style::Default, settings);
	window.setFramerateLimit(350);
#if _Debug_
	ImGui::SFML::Init(window, true);
	ImGuiIO& IO = ImGui::GetIO();
	IO.Fonts->Clear();
	IO.Fonts->AddFontFromFileTTF("Data\\Fonts\\SackersGothicStd-Medium.otf", 13.5f);
	ImGui::SFML::UpdateFontTexture();
#endif
	Engine::DebuggingSystem::setWindow(window);
	Engine::Game g(window);
	g.startGame();
	g.update();
	return 0;
}
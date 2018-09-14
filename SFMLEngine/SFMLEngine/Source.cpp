#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "DebuggingSystem.h"
#include <cmath>
using namespace std;

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
	ImGui::SFML::Init(window, true);
	Engine::DebuggingSystem::setWindow(window);
	Engine::Game g(window);
	g.startGame();
	g.update();

	return 0;
}
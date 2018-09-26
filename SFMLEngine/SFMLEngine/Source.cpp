#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "DebuggingSystem.h"
#include <cmath>
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h>
#include <crtdbg.h>  
using namespace std;

//#main
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
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
    #endif
	Engine::DebuggingSystem::setWindow(window);
	Engine::Game g(window);
	g.startGame();
	g.update();
	return 0;
}
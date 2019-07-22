#include <iostream>
#include <SFML/Graphics.hpp>
#include "DebuggingSystem.h"
#include <cmath>
#include <stdlib.h>
#include <entityx/Event.h>
#include "Game.h"
using namespace Engine;

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 16;
	settings.majorVersion = 3;
	settings.minorVersion = 0;

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Engine V0.3", sf::Style::Default);
	window.setFramerateLimit(350);

	ImGui::SFML::Init(window, true);
	ImGuiIO& IO = ImGui::GetIO();
	IO.Fonts->Clear();
	IO.Fonts->AddFontFromFileTTF("Data\\Fonts\\ArialRegular.ttf", 16.f);
	ImGui::SFML::UpdateFontTexture();

	Engine::DebuggingSystem::setWindow(window);
	
	Game g(window);
	g.start();

	return 0;
}
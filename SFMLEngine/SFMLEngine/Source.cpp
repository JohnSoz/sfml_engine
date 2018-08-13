#include <iostream>
#include <SFML/Graphics.hpp>
#include "World.h"
#include "DebuggingSystem.h"
using namespace std;

int main() 
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 16;
	settings.majorVersion = 3;
	settings.minorVersion = 0;

	sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML Game V0.00001",sf::Style::Default,settings);
#if Debug 
	ImGui::SFML::Init(window);
	Engine::Debugging::DebuggingSystem::setWindow(window);
#endif
	Engine::World world(window);
	world.startGame();

	auto h = world.getObjHendler();
	return 0;
}































































/*
int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
	Rectangle rect1(250, 80, 50, 100);
	Rectangle rect2(210, 125, 300, 50);

	sf::RectangleShape rectangle1;
	rectangle1.setSize(sf::Vector2f(rect1.w,rect1.h));
	rectangle1.setPosition(rect1.x, rect1.y);
	rectangle1.setFillColor(sf::Color::Blue);
	//rectangle1.setOrigin(sf::Vector2f(80,50));

	sf::RectangleShape rectangle2;
	rectangle2.setSize(sf::Vector2f(rect2.w, rect2.h));
	rectangle2.setPosition(rect2.x, rect2.y);
	rectangle2.setFillColor(sf::Color::Red);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::B)
				{
					auto z = Rectangle::GetIntersectionDepth(rect1, rect2);
					rect1.x += z.x;
					rect1.y += z.y;
					rectangle1.move(z.x, z.y);
				}
				if (event.key.code == sf::Keyboard::W)
				{
					rect1.x += 0;
					rect1.y += -1;
					rectangle1.move(0, -1);
				}
				else if (event.key.code == sf::Keyboard::S)
				{
					rect1.x += 0;
					rect1.y += 1;
					rectangle1.move(0, 1);
				}
				else if (event.key.code == sf::Keyboard::A)
				{
					rect1.x += -1;
					rect1.y += 0;
					rectangle1.move(-1, 0);
				}
				else if (event.key.code == sf::Keyboard::D)
				{
					rect1.x += 1;
					rect1.y += 0;
					rectangle1.move(1, 0);
				}
			}

		}

		window.clear();
		window.draw(rectangle1);
		window.draw(rectangle2);
		window.display();
	}

	return 0;
}*/
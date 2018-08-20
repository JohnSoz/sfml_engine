#include <iostream>
#include <SFML/Graphics.hpp>
#include "World.h"
#include "DebuggingSystem.h"
using namespace std;
#include <cmath>
int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 16;
	settings.majorVersion = 3;
	settings.minorVersion = 0;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Game V0.00001", sf::Style::Default, settings);
	#if Debug
	ImGui::SFML::Init(window, true);
	Engine::DebuggingSystem::setWindow(window);
	#endif
	Engine::World world(window);
	world.startGame();

	//float angle = 0;
	//sf::CircleShape shape2;
	//shape2.setFillColor(sf::Color::Red);
	//shape2.setRadius(2);

	//sf::FloatRect zasd(50, 50, 130, 130);
	//sf::Transformable t;
	//t.setOrigin(sf::Vector2f(80, 80));
	//t.setPosition(50, 50);
	//t.rotate(10);
	//auto result = t.getTransform().transformRect(zasd);

	//while (window.isOpen())
	//{
	//	sf::Event event;
	//	while (window.pollEvent(event))
	//	{
	//		if (event.type == sf::Event::Closed)
	//			window.close();
	//		if (event.type == sf::Event::KeyPressed)
	//		{
	//			if (event.key.code == sf::Keyboard::B)
	//			{
	//				angle++;
	//			}
	//		}
	//	}
	//	auto x = 800;
	//	auto y = 400;
	//	auto w = 300;
	//	auto h = 300;
	//	float centerX = x + abs(x - (w + x)) / 2;
	//	float centerY = y + abs(y - (h + y)) / 2;
	//	shape2.setPosition(centerX, centerY);
	//	float z = angle * 3.141592653589793 / 180.0;
	//	//X = x0 + (x - x0) * cos(a) - (y - y0) * sin(a);
	//	//Y = y0 + (y - y0) * cos(a) + (x - x0) * sin(a);
	//	//sf::Vertex line[] =
	//	//{
	//	//	sf::Vertex(sf::Vector2f(centerX + (x - centerX) * cos(z) - (y - centerY)*sin(z) , centerY + (y - centerY) * cos(z) + (x - centerX) * sin(z))),//x,y
	//	//	sf::Vertex(sf::Vector2f(centerX + ((x + w) - centerX) * cos(z) - (y - centerY) * sin(z) , centerY + (y - centerY) * cos(z) + ((x + w) - centerX) * sin(z))),//w,y
	//	//	sf::Vertex(sf::Vector2f(centerX + ((x + w) - centerX) * cos(z) - ((y + h) - centerY) * sin(z) , centerY + ((y + h) - centerY) * cos(z) + ((x + w) - centerX) * sin(z))),//w,h
	//	//	sf::Vertex(sf::Vector2f(centerX + (x - centerX) * cos(z) - ((y + h) - centerY)*sin(z) ,  centerY + ((y + h) - centerY) * cos(z) + (x - centerX) * sin(z))),//x,h
	//	//	sf::Vertex(sf::Vector2f(centerX + (x - centerX) * cos(z) - (y - centerY) * sin(z) , centerY + (y - centerY) * cos(z) + (x - centerX) * sin(z)))//x,y
	//	//};
	//	//shape.setPosition(sf::Vector2f(centerX + (x - centerX) * cos(z) - (y - centerY)*sin(z), centerY + (y - centerY)*cos(z) + (x - centerX)*sin(z)));
	//	window.clear();
	//	//window.draw(line, 5, sf::LinesStrip);
	//	window.draw(rect);
	//	window.display();
	//}

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
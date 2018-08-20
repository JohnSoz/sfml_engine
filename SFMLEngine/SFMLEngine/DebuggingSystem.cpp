#include "DebuggingSystem.h"

using namespace Engine;
sf::RenderWindow* Engine::DebuggingSystem::window = nullptr;
void DebuggingSystem::draw()
{
	//std::vector<sf::Vertex*> rects1;
	//std::vector<sf::VertexArray> rects2;
	for (auto &i : entites)
	{
		//auto x = i.second->x - 90;
		//auto y = i.second->y - 120;
		auto x = i.first->x;
		auto y = i.first->y;
		auto w = i.first->w;
		auto h = i.first->h;

		auto x2 = i.second->x;
		auto y2 = i.second->y;
		auto w2 = i.second->w;
		auto h2 = i.second->h;

		auto mousePos = sf::Mouse::getPosition(*window);
		sf::Vertex vertices[5] =
		{
				sf::Vertex(sf::Vector2f(x, y), sf::Color::Red),
				sf::Vertex(sf::Vector2f(w2, h2), sf::Color::Blue),
				sf::Vertex(sf::Vector2f(w, h), sf::Color::Red),
				sf::Vertex(sf::Vector2f(x2, y2), sf::Color::Blue),
				sf::Vertex(sf::Vector2f(x, y), sf::Color::Red)
		};
		sf::RectangleShape shape;
		shape.setPosition(w2, h2);
		shape.setFillColor(sf::Color::White);
		shape.setSize(sf::Vector2f(10, 10));
		shape.setOrigin(sf::Vector2f(5, 5));
		//	if ((mousePos.x >= x && mousePos.x <= w) && (mousePos.y >= y && mousePos.y <= h))
		//	{
		ImGUI::SimpleText(sf::Vector2f(x - 102, y - 70), &overlay, "Window" + std::to_string(x + y2));
		ImGUI::SimpleText(sf::Vector2f(w, h), &overlay, "Window" + std::to_string(y + x2));
		//	}
		window->draw(vertices, 5, sf::LinesStrip);
		window->draw(shape);
	}

	for (auto i : obj)
	{
		auto x = i.left;
		auto y = i.top;
		auto w = i.width;
		auto h = i.height;

		sf::VertexArray triangle(sf::LinesStrip, 5);
		triangle[0] = sf::Vector2f(x, y);
		triangle[0].color = sf::Color::Blue;

		triangle[1].position = sf::Vector2f(x + w, y);
		triangle[1].color = sf::Color::Blue;

		triangle[2].position = sf::Vector2f(x + w, y + h);
		triangle[2].color = sf::Color::Blue;

		triangle[3].position = sf::Vector2f(x, y + h);
		triangle[3].color = sf::Color::Blue;

		triangle[4].position = sf::Vector2f(x, y);
		triangle[4].color = sf::Color::Blue;

		ImGUI::SimpleText(sf::Vector2f(x, y), &overlay, "Window" + std::to_string(x + h));
		ImGUI::SimpleText(sf::Vector2f(x + w, y + h), &overlay, "Window" + std::to_string(y + w));
		window->draw(triangle);
	}
}

void Engine::DebuggingSystem::handleEvent(sf::Event& event)
{
	if (event.type = sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::T)
		{
			if (Pressclock.getElapsedTime().asMilliseconds() > 500)
			{
				overlay = !overlay;
				Pressclock.restart();
			}
		}
	}
}

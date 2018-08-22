#include "DebuggingSystem.h"

using namespace Engine;
sf::RenderWindow* Engine::DebuggingSystem::window = nullptr;
void DebuggingSystem::draw()
{

	int count = 0;
	for (auto &i : entites) //Оптимизировать
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

		sf::VertexArray triangle(sf::LinesStrip, 5);
		triangle[0] = sf::Vector2f(x, y);
		triangle[0].color = sf::Color::Red;

		triangle[1].position = sf::Vector2f(w2, h2);
		triangle[1].color = sf::Color::Blue;

		triangle[2].position = sf::Vector2f(w, h);
		triangle[2].color = sf::Color::Red;

		triangle[3].position = sf::Vector2f(x2, y2);
		triangle[3].color = sf::Color::Blue;

		triangle[4].position = sf::Vector2f(x, y);
		triangle[4].color = sf::Color::Red;

		ImGUI::SimpleText(sf::Vector2f(x - 102, y - 70), &overlay, "D_Window_" + std::to_string(count));
		count++;
		ImGUI::SimpleText(sf::Vector2f(w, h), &overlay, "D_Window_" + std::to_string(count));
		count++;

		window->draw(triangle);
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

		ImGUI::SimpleText(sf::Vector2f(x, y), &overlay, "D_Window_" + std::to_string(count));
		count++;
		ImGUI::SimpleText(sf::Vector2f(x + w, y + h), &overlay, "D_Window_" + std::to_string(count));
		count++;

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

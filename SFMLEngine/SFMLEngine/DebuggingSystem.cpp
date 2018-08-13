#include "DebuggingSystem.h"

using namespace Engine::Debugging;
sf::RenderWindow* Engine::Debugging::DebuggingSystem::window = nullptr;

void DebuggingSystem::draw()
{
	std::vector<sf::Vertex*> rects;
	for (auto &i : entites)
	{
		auto x = i.second.x;
		auto y = i.second.y;
		auto w = i.first.w;
		auto h = i.first.h;
		sf::Vertex vertices[5] =
		{
			sf::Vertex(sf::Vector2f(x, y), sf::Color::Red),
			sf::Vertex(sf::Vector2f(x + w, y), sf::Color::Red),
			sf::Vertex(sf::Vector2f(x + w, y + h), sf::Color::Red),
			sf::Vertex(sf::Vector2f(x, y + h), sf::Color::Red),
			sf::Vertex(sf::Vector2f(x, y), sf::Color::Red)
		};
		rects.push_back(vertices);
	}
	for (auto &i : rects)
		window->draw(i, 5, sf::LinesStrip);
}
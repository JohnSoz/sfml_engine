#include "DebuggingSystem.h"

using namespace Engine::Debugging;
sf::RenderWindow* Engine::Debugging::DebuggingSystem::window = nullptr;

void DebuggingSystem::draw()
{
	std::vector<sf::Vertex*> rects1;
	std::vector<sf::VertexArray> rects2;

	for (auto &i : entites)
	{
		auto x = i.second->x - 90;
		auto y = i.second->y - 120;
		auto w = i.first->w;
		auto h = i.first->h;
		sf::Vertex vertices[5] =
		{
			sf::Vertex(sf::Vector2f(x, y), sf::Color::Red),
			sf::Vertex(sf::Vector2f(x + w, y), sf::Color::Red),
			sf::Vertex(sf::Vector2f(x + w, y + h), sf::Color::Red),
			sf::Vertex(sf::Vector2f(x, y + h), sf::Color::Red),
			sf::Vertex(sf::Vector2f(x, y), sf::Color::Red)
		};
		rects1.push_back(vertices);
	}
	for (auto& i : obj)
	{
		auto x = i->left;
		auto y = i->top;
		auto w = i->width;
		auto h = i->height;

		sf::VertexArray triangle(sf::LineStrip, 5);
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

	/*	sf::Vertex vertices[] =
		{
			 sf::Vertex(sf::Vector2f(x, y), sf::Color::Blue),
			 sf::Vertex(sf::Vector2f(x + w, y), sf::Color::Blue),
			 sf::Vertex(sf::Vector2f(x + w, y + h), sf::Color::Blue),
			 sf::Vertex(sf::Vector2f(x, y + h), sf::Color::Blue),
			 sf::Vertex(sf::Vector2f(x, y), sf::Color::Blue)
		};*/
		rects2.push_back(triangle);
	}
	for (auto &i : rects1)
		window->draw(i, 5, sf::LinesStrip);
	for (auto &i : rects2)
		window->draw(i);
}
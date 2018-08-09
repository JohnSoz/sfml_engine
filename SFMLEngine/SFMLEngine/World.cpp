#include "World.h"
using namespace Engine;
float World::time = 0;

Entity& Engine::ObjectHandler::GetObjects(std::string NAME)
{
	Entity* retObj = nullptr;
	for (auto &item : ObjectsArray)
	{
		if (item->name == NAME)
			retObj = item;
	}
	return *retObj;
}

void Engine::ObjectHandler::PushObject(Entity* obj)
{
	ObjectsArray.shrink_to_fit();
	ObjectsArray.push_back(obj);
}

void Engine::ObjectHandler::UpdateObjects(float time)
{
	for (auto& o : ObjectsArray)
	{
		o->update(World::getTime());
	}
}

void Engine::ObjectHandler::RenderObjects(sf::RenderWindow & WINDOW)
{
	for (auto& o : ObjectsArray)
	{
		WINDOW.draw(o->sprite);
	}
}
void Engine::World::update()
{
	while (window->isOpen())
	{
		float time = mainClock.getElapsedTime().asMicroseconds();
		mainClock.restart();
		time = time / 800;
		sf::Event event;
		//std::cout << time << std::endl;
		handleEvent(event);
		objHandler.UpdateObjects(time);
		draw();
	}
}

void Engine::World::handleEvent(sf::Event & event)
{
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;

		case sf::Event::KeyPressed:

			break;

		case sf::Event::KeyReleased:

			break;

		default:
			break;
		}
	}
}

void Engine::World::draw()
{
	window->clear();
	objHandler.RenderObjects(*window);
	window->display();
}

void Engine::World::init()
{
	sf::Image i;
	i.loadFromFile("Data/OSprite/AnimTile.png");
	objHandler.PushObject(new Engine::Actor(i, Vector2D(12, 12), Rectangle(0, 0, 200,200), "Test"));
}

void Engine::World::startGame()
{
	init();
	 objHandler.GetObjects("Test").addComponent<PositionComponent>();
	 auto z = objHandler.GetObjects("Test").getComponent<PositionComponent>();
	 z->SetPos();
	update();
}

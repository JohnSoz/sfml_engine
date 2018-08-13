#include "World.h"
using namespace Engine;
float World::time = 0;
template<class Obj>
Obj& Engine::ObjectHandler::GetObjects(std::string NAME)
{
	Entity* retObj = nullptr;
	for (auto &item : ObjectsArray)
	{
		if (item->name == NAME)
			retObj = item;
	}
	return *static_cast<Obj*>(retObj);
}

void Engine::ObjectHandler::PushObject(Entity* obj)
{
	ObjectsArray.shrink_to_fit();
#if Debug
	debug.pushEntites(*obj);
#endif 
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
#if Debug 
		debug.draw();
#endif
		WINDOW.draw(o->sprite);
	}
}
void Engine::World::update()
{
	sf::Clock deltaClock;
	while (window->isOpen())
	{
		time = mainClock.getElapsedTime().asMicroseconds();
		mainClock.restart();
		time = time / 300;
		sf::Event event;
		handleEvent(event);
		objHandler.GetObjects<Actor>("Test").handleEvent(event);
		objHandler.UpdateObjects(time);
		draw();
#if Debug 
		//ImGui::SFML::Update(*window, deltaClock.restart());
		//ImGUI::SimpleOverlay(sf::Vector2f(12, 12), &ShowOverlay);
#endif
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
#if Debug 
		//ImGui::SFML::ProcessEvent(event);
#endif
	}
}

void Engine::World::draw()
{
	window->clear(sf::Color::White);
	level.DrawLevel(*window);
	objHandler.RenderObjects(*window);
#if Debug 
	//if (ShowOverlay)
	//	ImGui::SFML::Render(*window);
#endif
	window->display();
}

void Engine::World::init()
{
	view.reset(sf::FloatRect(0, 0, 1920, 1080));
	window->setView(view);
	level.LoadFromFile("Data/Level/map5.tmx", 2);
	objHandler.debug.levelObjects(level.GetAllObjects());
	sf::Image i;
	i.loadFromFile("Data/OSprite/AnimTile.png");
	objHandler.PushObject(new Engine::Actor(i, Vector2D(100, 120), Engine::Rectangle(40, 40, 210, 150), "Test", *window, level));
}

void Engine::World::startGame()
{
	init();
	update();
}

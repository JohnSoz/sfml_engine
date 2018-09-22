#include "World.h"
#include "LogConsole.h"
#include <algorithm>
using namespace Engine;

template<class Obj>
Obj& Engine::ObjectHandler::GetObjects(std::string NAME)
{
	auto retObj = std::find_if(ObjectsArray.begin(), ObjectsArray.end(),
		[NAME](const Entity* e1)->bool
	{
		return (e1->name == NAME) ? true : false;
	});
	return *static_cast<Obj*>(*retObj);
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
		o->update(time);
	}
}

void Engine::ObjectHandler::RenderObjects(sf::RenderWindow & WINDOW)
{
	for (auto& o : ObjectsArray)
	{
		WINDOW.draw(o->sprite);
	}
}

void Engine::ObjectHandler::refresh()
{
	ObjectsArray.erase(std::remove_if(std::begin(ObjectsArray), std::end(ObjectsArray),
		[](const Entity *entity)->bool
	{
		return !entity->isActive();
	}));
}


void Engine::World::update(sf::RenderWindow & window, float time, sf::Event& event)
{
	objHandler.GetObjects<Actor>("Test").handleEvent(event);
	objHandler.UpdateObjects(time);
	if (ShowOverlay)
	{
		ImGUI::SimpleOverlay(&ShowOverlay);
	}
	//draw(window);
}


void Engine::World::handleEvent(sf::Event & event)
{
	debug.handleEvent(event);
}

void Engine::World::draw(sf::RenderWindow & window)
{
	//level.DrawLevel(*window);
	//sf::Sprite sprite(level.DrawLevel2());
	window.draw(LevelSprite);
	objHandler.RenderObjects(window);
	debug.draw();
}


void Engine::World::Init(sf::RenderWindow & window)
{
	level.LoadFromFile("Data/Level/map5.tmx", 2);
	LevelTexture.loadFromImage(*level.DrawLevel2());
	LevelSprite.setTexture(LevelTexture);
	debug.levelObjects(level.GetAllObjects());

	///TEST
	Console::AppLog::addLog(Console::Log("Engine::World::Init()", Console::logType::info));

	sf::Image i;
	i.loadFromFile("Data/OSprite/AnimTile.png");
	pushEntity(new Engine::Actor(i, sf::Vector2f(100, 120), sf::IntRect(38, 39, 209, 154), "Test", window, level));
}

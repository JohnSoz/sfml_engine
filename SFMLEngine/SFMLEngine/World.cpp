#include "World.h"
#include "LogConsole.h"
#include <algorithm>
#include "Player.h"
#include "serializer.h"
#include "EngineEvents.h"
using namespace Engine;

void Engine::World::update(sf::RenderWindow& window, float time)
{
	Player* p = objHandler.GetObjects<Player>("Test");
	p->isKeyPressed();
	cam.moveToPoint(p->getPos(), time, { 1.f ,1.f });
	objHandler.UpdateObjects(time);
	objHandler.CollisionUpdate();
	window.setView(cam.getView());
}

void World::fixedUpdate()
{
	if (ShowOverlay)
		ImGUI::SimpleOverlay(&ShowOverlay);
	objHandler.fixedUpdate();
}

void Engine::World::load(sf::RenderWindow& window)
{
	Console::AppLog::addLog(Console::Log("Engine::World::load()", Console::logType::info));
	pushEntity(new Engine::Player(window, level));
}

Entity* Engine::World::pushEntity(Entity* e)
{
	debug.pushRectangle(e->getDebugRect());
	return objHandler.PushObject(e);
}

void Engine::World::handleEvent(sf::Event& event)
{
	debug.handleEvent(event);
	objHandler.handelEvent(event);
}

void Engine::World::draw(sf::RenderWindow& window)
{
	window.draw(LevelSprite);
	objHandler.RenderObjects(window);
	debug.draw();
	objHandler.GetObjects<Player>("Test")->draw();
}

void Engine::World::Init(sf::RenderWindow& window)
{
	level.LoadFromFile("Data/Level/shpiga_test_level.tmx");
	this->LevelTexture.loadFromImage(level.DrawLevel2());
	this->LevelSprite.setTexture(LevelTexture);
	debug.levelObjects(level.GetAllObjects());
	Console::AppLog::addLog(Console::Log("Engine::World::Init()", Console::logType::info));
}


Engine::World::World()
{
	ShowOverlay = true;
	Engine::EventManager::eventManager.subscribe<Events::NewObject_Event<Bullet>>(*this);
	Engine::EventManager::eventManager.subscribe<Events::NewObject_Event<Entity>>(*this);
}

void Engine::World::start(sf::RenderWindow& window) ///depricated!!!!!!!!!!!!
{
	pushEntity(new Engine::Player(sf::Vector2f(120, 120), "Test", window, level, "Data\\Animation\\TestAnim.xml"));
}

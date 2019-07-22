#include "World.h"
#include "LogConsole.h"
#include <algorithm>
#include "Player.h"
#include "serializer.h"
using namespace Engine;

//Level Engine::World::level;

Engine::ObjectHandler::~ObjectHandler()
{
	for (iter = ObjectsArray.begin(); iter != ObjectsArray.end(); ++iter)
	{
		delete (*iter);
	}
	ObjectsArray.clear();
}

void Engine::ObjectHandler::handelEvent(sf::Event& e)
{
	for (auto& o : ObjectsArray)
	{
		if (o->type == OActor)
			static_cast<Actor*>(o)->handleEvent(e);
	}
}

template<class Obj>
Obj* Engine::ObjectHandler::GetObjects(std::string NAME)
{
	auto retObj = std::find_if(ObjectsArray.begin(), ObjectsArray.end(),
		[NAME](const Entity* e1)->bool
		{
			return (e1->name == NAME);
		});
	return static_cast<Obj*>(*retObj);
}

Entity* Engine::ObjectHandler::PushObject(Entity* obj)
{
	string info = "Engine::ObjectHandler::PushObject(" + obj->getName() + ")";
	Console::AppLog::addLog(info, Console::info);
	ObjectsArray.push_back(obj);
	iter = ObjectsArray.begin();
	return *iter;
}

void Engine::ObjectHandler::UpdateObjects(float time)
{
	for (iter = ObjectsArray.begin(); iter != ObjectsArray.end();)
	{
		(*iter)->update(time);
		if ((*iter)->IsActive == false)
		{
			delete (*iter);
			iter = ObjectsArray.erase(iter);
		}
		else
			++iter;
	}
}

void Engine::ObjectHandler::CollisionUpdate()
{
	/*for (iter = ObjectsArray.begin(); iter != ObjectsArray.end(); iter++)
	{
		for (auto iter2 = iter + 1; iter2 != ObjectsArray.end(); iter2++)
		{
			if ((*iter)->getRect().intersects((*iter2)->getRect()))
			{
				(*iter)->CollisionUpdate(*iter2);
			}
		}
	}*/
}

void Engine::ObjectHandler::RenderObjects(sf::RenderWindow& WINDOW)
{
	for (auto& o : ObjectsArray)
	{
		WINDOW.draw(o->sprite);
		if (o->getType() == OPlayer)
		{
			auto z = static_cast<Actor*>(o);
			VertexArray vs(Lines, 2);
			vs[0].position = { z->getPos().x - z->getOrigin().x * z->getScale() + 1 ,z->getPos().y - z->getSprite().getLocalBounds().height / 4.f };
			vs[1].position = z->ray;
			vs[0].color = sf::Color::Green;
			vs[1].color = sf::Color::White;
			WINDOW.draw(vs);

			VertexArray vs2(Lines, 2);
			vs2[0].position = { z->getPos().x + z->getOrigin().x * z->getScale() - 1 ,z->getPos().y - z->getSprite().getLocalBounds().height / 4.f };
			vs2[1].position = z->ray2;
			vs2[0].color = sf::Color::White;
			vs2[1].color = sf::Color::Green;
			WINDOW.draw(vs2);

			sf::CircleShape shape2(1);
			shape2.setFillColor(sf::Color::Blue);
			//shape2.setOrigin(1, 1);
			shape2.setPosition(z->sprite.getPosition());
			WINDOW.draw(shape2);
		}
	}
}

void Engine::ObjectHandler::refresh()
{
	Console::AppLog::addLog("Engine::ObjectHandler::refresh()", Console::info);
	ObjectsArray.erase(std::remove_if(std::begin(ObjectsArray), std::end(ObjectsArray),
		[](const Entity* entity)->bool
		{
			return !entity->isActive();
		}), ObjectsArray.end());
}


void Engine::World::update(sf::RenderWindow& window, float time)
{
	Player* p = objHandler.GetObjects<Player>("Test");
	p->isKeyPressed();
	cam.moveToPoint(p->getPos(), time, { 1.f ,1.f });

	objHandler.UpdateObjects(time);
	objHandler.CollisionUpdate();
	window.setView(cam.getView());
}

void World::updateImGui()
{
	if (ShowOverlay)
		ImGUI::SimpleOverlay(&ShowOverlay);
	Player* p = objHandler.GetObjects<Player>("Test");
	debug::debugDraw<Player, Object, Actor, Entity>(p, "Debug For Class Player");
}

void Engine::World::load(sf::RenderWindow& window)
{
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
	objHandler.GetObjects<Player>("Test")->handleEvent(event);
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
	level.w = &window;
	window.setActive(true);
	level.LoadFromFile("Data/Level/shpiga_test_level.tmx");
	this->LevelTexture.loadFromImage(level.DrawLevel2());
	this->LevelSprite.setTexture(LevelTexture);
	window.setActive(false);
	debug.levelObjects(level.GetAllObjects());
	Console::AppLog::addLog(Console::Log("Engine::World::Init()", Console::logType::info));
}

#include "EngineEvents.h"
Engine::World::World()
{
	ShowOverlay = true;
	Engine::EventManager::eventManager.subscribe<Events::NewObject_Event>(*this);
}

void Engine::World::receive(const Events::NewObject_Event& entity)
{
	entity.obj->setLevel(level);
	pushEntity(entity.obj);
}

void Engine::World::start(sf::RenderWindow& window)
{
	pushEntity(new Engine::Player(sf::Vector2f(120, 120), "Test", window, level, "Data\\Animation\\TestAnim.xml"));
}

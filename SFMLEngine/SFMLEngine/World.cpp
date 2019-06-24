#include "World.h"
#include "LogConsole.h"
#include <algorithm>
#include "Player.h"
using namespace Engine;

Level Engine::World::level;

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
		return (e1->name == NAME) ? true : false;
	});
	return static_cast<Obj*>(*retObj);
}

void Engine::ObjectHandler::PushObject(Entity* obj)
{
	string info = "Engine::ObjectHandler::PushObject(" + obj->getName() + ")";
	Console::AppLog::addLog(info, Console::info);
	ObjectsArray.push_back(obj);
	iter = ObjectsArray.begin();
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



void Engine::ObjectHandler::RenderObjects(sf::RenderWindow& WINDOW)
{
	for (auto& o : ObjectsArray)
	{
		WINDOW.draw(o->sprite);
		if (o->getType() == OActor)
		{
			auto z = static_cast<Actor*>(o);
			VertexArray vs(Lines, 2);
			vs[0].position = { z->getPos().x - z->getRect().width + z->getSprite().getOrigin().x / 2 + 1,z->getPos().y };
			vs[1].position = z->ray;
			vs[0].color = sf::Color::Green;
			vs[1].color = sf::Color::White;
			WINDOW.draw(vs);
			VertexArray vs2(Lines, 2);
			vs2[0].position = { z->getPos().x + z->getRect().width - z->getSprite().getOrigin().x / 2 - 1,z->getPos().y };
			vs2[1].position = z->ray2;
			vs2[0].color = sf::Color::White;
			vs2[1].color = sf::Color::Green;
			WINDOW.draw(vs2);
			sf::CircleShape shape2(2);
			shape2.setFillColor(sf::Color::Blue);
			shape2.setOrigin(1, 1);
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


void Engine::World::update(sf::RenderWindow& window, float time, sf::Event& event)
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
	{
		ImGUI::SimpleOverlay(&ShowOverlay);
	}
	Player* p = objHandler.GetObjects<Player>("Test");
	debug::debugDraw<Player, Object, Actor>(p, "Debug For Class Player");
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
	level.LoadFromFile("Data/Level/shpiga_test_level.tmx");
	this->LevelTexture.loadFromImage(level.DrawLevel2());
	this->LevelSprite.setTexture(LevelTexture);
	debug.levelObjects(level.GetAllObjects());
	///TEST
	Console::AppLog::addLog(Console::Log("Engine::World::Init()", Console::logType::info));

	pushEntity(new Engine::Player(sf::Vector2f(120, 120), "Test", window, level, "Animation.xml"));
}

#include "EngineEvents.h"
Engine::World::World()
{
	renderTexture.create(1920, 1080);
	ShowOverlay = true;
	Engine::EventManager::eventManager.subscribe<Events::NewObject_Event>(*this);
}

void Engine::World::receive(const Events::NewObject_Event& entity)
{
	pushEntity(entity.obj);
}

void Engine::World::start()
{
	objHandler.callStart();
}

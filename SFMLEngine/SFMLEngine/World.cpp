#include "World.h"
#include "LogConsole.h"
#include <algorithm>
using namespace Engine;

void Engine::ObjectHandler::handelEvent(sf::Event& e)
{
	for (auto& o : ObjectsArray)
	{
		if (o->type == OActor)
			static_cast<Actor*>(o)->handleEvent(e);
	}
}

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
	string info = "Engine::ObjectHandler::PushObject(" + obj->getName() + ")";
	Console::AppLog::addLog(info, Console::info);
	ObjectsArray.shrink_to_fit();
	ObjectsArray.push_back(obj);
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

void Engine::ObjectHandler::RenderObjects(sf::RenderWindow & WINDOW)
{
	for (auto& o : ObjectsArray)
	{
		WINDOW.draw(o->sprite);
		if (o->getType() == OActor)
		{
			auto z = static_cast<Actor*>(o);
			sf::CircleShape shape(1);
			sf::CircleShape shape2(2);
			shape.setFillColor(sf::Color::Blue);
			shape2.setFillColor(sf::Color::Red);
			shape2.setOrigin(1, 1);
			sf::Vector2f pos = z->getPointOfFire();
			shape.setPosition(pos);
			shape2.setPosition(z->sprite.getPosition());
			WINDOW.draw(shape);
			WINDOW.draw(shape2);
		}
	}
}

void Engine::ObjectHandler::refresh()
{
	Console::AppLog::addLog("Engine::ObjectHandler::refresh()", Console::info);
	ObjectsArray.erase(std::remove_if(std::begin(ObjectsArray), std::end(ObjectsArray),
		[](const Entity *entity)->bool
	{
		return !entity->isActive();
	}), ObjectsArray.end());
}


void Engine::World::update(sf::RenderWindow & window, float time, sf::Event& event)
{
	objHandler.GetObjects<Actor>("Test").isKeyPressed();
	if (auto z = objHandler.GetObjects<Actor>("Test").shotUpdate(level); z != nullptr)
	{
		pushEntity(z);
	}
	objHandler.UpdateObjects(time);
	objHandler.CollisionUpdate();
	if (ShowOverlay)
	{
		ImGUI::SimpleOverlay(&ShowOverlay);
	}
}


void Engine::World::handleEvent(sf::Event & event)
{
	debug.handleEvent(event);
	objHandler.GetObjects<Actor>("Test").handleEvent(event);
}

void Engine::World::draw(sf::RenderWindow & window)
{
	window.draw(LevelSprite);
	objHandler.RenderObjects(window);
	debug.draw();
	objHandler.GetObjects<Actor>("Test").draw();
}


void Engine::World::Init(sf::RenderWindow & window)
{
	level.LoadFromFile("Data/Level/map5.tmx");
	this->LevelTexture.loadFromImage(level.DrawLevel2());
	this->LevelSprite.setTexture(LevelTexture);
	debug.levelIsoObjects(level.GetAllObjects());
	debug.levelObjects(level.GetAllObjects());
	///TEST
	Console::AppLog::addLog(Console::Log("Engine::World::Init()", Console::logType::info));

	sf::Image i;
	i.loadFromFile("Data/OSprite/nandGunMove.png");
	pushEntity(new Engine::Actor(i, sf::Vector2f(120, 120), "Test", window, level));

	sf::Image i2;
	i2.loadFromFile("Data/OSprite/nandGunMove.png");
	pushEntity(new Engine::Test(i2, sf::IntRect(1, 30, 190, 140), sf::Vector2f(400, 120), "Test2"));
}

void Engine::World::start()
{
	objHandler.callStart();
}

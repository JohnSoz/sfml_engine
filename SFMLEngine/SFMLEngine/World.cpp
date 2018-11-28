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
			//shape.setOrigin(2, 2);
			shape.setFillColor(sf::Color::Blue);
			sf::Vector2f pos = z->getPointOfFire();
			shape.setPosition(pos);
			WINDOW.draw(shape);
		}
	}
}

void Engine::ObjectHandler::refresh()
{
	ObjectsArray.erase(std::remove_if(std::begin(ObjectsArray), std::end(ObjectsArray),
		[](const Entity *entity)->bool
	{
		return !entity->isActive();
	}), ObjectsArray.end());
}


void Engine::World::update(sf::RenderWindow & window, float time, sf::Event& event)
{
	objHandler.GetObjects<Actor>("Test").handleEvent(event);
	objHandler.UpdateObjects(time);
	objHandler.CollisionUpdate();
	if (ShowOverlay)
	{
		ImGUI::SimpleOverlay(&ShowOverlay);
	}
	//draw(window);
}


void Engine::World::handleEvent(sf::Event & event)
{
	debug.handleEvent(event);
	//objHandler.handelEvent(event);
	objHandler.GetObjects<Actor>("Test").invHandleEvent(event);
	Console::AppLog::addLog("shoot", Console::logType::info);
	if (auto z = objHandler.GetObjects<Actor>("Test").shotUpdate(level); z != nullptr)
	{
		pushEntity(z);
	}
	/*
	sf::Image i;
	i.loadFromFile("Data/images/bullet.png");
	pushEntity(objHandler.GetObjects<Actor>("Test").shot(level));
	gunClock.restart();
	*/

}

void Engine::World::draw(sf::RenderWindow & window)
{
	//level.DrawLevel(*window);
	//sf::Sprite sprite(level.DrawLevel2());
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

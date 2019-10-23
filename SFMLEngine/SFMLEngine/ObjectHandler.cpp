#include "ObjectHandler.h"
#include "Actor.h"
using namespace Engine;

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
		if (o->getType() > ObjectType::OEntity)
			static_cast<Actor*>(o)->handleEvent(e);
	}
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
		if ((*iter)->isActive() == false)
		{
			delete (*iter);
			iter = ObjectsArray.erase(iter);
		}
		else
			++iter;
	}
}

void Engine::ObjectHandler::fixedUpdate()
{
	for (iter = ObjectsArray.begin(); iter != ObjectsArray.end();)
	{
		(*iter)->fixedUpdate();
		if ((*iter)->isActive() == false)
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
	for (auto o : ObjectsArray)
	{
		WINDOW.draw(*o);
		if (o->getType() == ObjectType::OPlayer)
		{
			auto z = static_cast<Actor*>(o);
			VertexArray vs(Lines, 2);
			vs[0].position = { z->getPos().x - z->getOrigin().x,z->getPos().y - 1.f };
			vs[1].position = z->ray;
			vs[0].color = sf::Color::Green;
			vs[1].color = sf::Color::White;
			WINDOW.draw(vs);

			VertexArray vs2(Lines, 2);
			vs2[0].position = { z->getPos().x + z->getOrigin().x,z->getPos().y - 1.f };
			vs2[1].position = z->ray2;
			vs2[0].color = sf::Color::White;
			vs2[1].color = sf::Color::Green;
			WINDOW.draw(vs2);

			sf::CircleShape shape2(1);
			shape2.setFillColor(sf::Color::Blue);
			//shape2.setOrigin(1, 1);
			shape2.setPosition({ z->sprite.getPosition().x ,z->sprite.getPosition().y });
			WINDOW.draw(shape2);
		}
	}
}

bool Engine::ObjectHandler::hasObject(std::string_view name)
{
	auto obj = std::find_if(ObjectsArray.begin(), ObjectsArray.end(), [name](const auto Obj) {return name == Obj->getName(); });
	return (obj == ObjectsArray.end()) ? false : true;
}

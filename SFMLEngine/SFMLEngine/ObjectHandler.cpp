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

void Engine::ObjectHandler::RenderObjects(sf::RenderWindow& w)
{
	for (auto o : ObjectsArray)
	{
		w.draw(*o);
		if (o->getType() == ObjectType::OPlayer)
		{
			auto z = static_cast<Actor*>(o);

			sf::Vector2f pos = { z->getPos().x - z->getLocalRect().width * z->getScale() / 2, z->getPos().y - z->getLocalRect().height * z->getScale() / 2 };
			sf::Vector2f pos2 = { z->getRect().left, z->getRect().top };

			sf::CircleShape c(1.f);
			c.setPosition(pos);
			c.setFillColor(sf::Color::Green);
			sf::CircleShape c2(1.f);
			c2.setPosition(pos2);
			c2.setFillColor(sf::Color::Red);

			w.draw(c2);
			w.draw(c);
			//w.draw(z->govno);
			w.draw(z->left);
			w.draw(z->right);
			w.draw(z->middle);
		}
	}
}

bool Engine::ObjectHandler::hasObject(std::string_view name)
{
	auto obj = std::find_if(ObjectsArray.begin(), ObjectsArray.end(), [name](const auto Obj) {return name == Obj->getName(); });
	return (obj == ObjectsArray.end()) ? false : true;
}

#pragma once
#include "DebuggingSystem.h"
#include "imgui.h"
#include "imgui-sfml.h"
#include "Actor.h"
#include <entityx/entityx.h>

namespace Engine
{
	class ObjectHandler
	{
	private:
		std::vector<Entity*>           ObjectsArray;
		std::vector<Entity*>::iterator iter;
	public:
		DebuggingSystem debug;
		ObjectHandler() = default;
		~ObjectHandler()
		{
			for (iter = ObjectsArray.begin(); iter != ObjectsArray.end(); ++iter)
			{
				delete (*iter);
			}
			ObjectsArray.clear();
		}

		void handelEvent(sf::Event& e);
		template<class Obj>
		Obj& GetObjects(std::string NAME);
		void PushObject(Entity* obj); //TODO:  Запретить добавлять объекты во время выполнения UpdateObjects
		void UpdateObjects(float time);
		void CollisionUpdate()
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
		void callStart()
		{
			Console::AppLog::addLog("Engine::ObjectHandler::callStart()", Console::info);
			for (iter = ObjectsArray.begin(); iter != ObjectsArray.end(); ++iter)
			{
				(*iter)->start();
			}
		}
		void RenderObjects(sf::RenderWindow& WINDOW);
		void refresh();
	};

	class World : public entityx::Receiver<World>
	{
	private:
		sf::Clock         mainClock, deltaClock;
		sf::Sprite        LevelSprite;
		sf::Texture       LevelTexture;
		ObjectHandler     objHandler;
        static Level      level;
		sf::RenderTexture renderTexture;
		DebuggingSystem debug;
		bool ShowOverlay;


		void Init(sf::RenderWindow& window);

	public:
		World();

		~World() = default;

		void handleEvent(sf::Event& event);

		void draw(sf::RenderWindow& window);

		void update(sf::RenderWindow& window, float time, sf::Event& event);

		void pushEntity(Entity* e)
		{
			debug.pushRectangle(e->getDebugRect());
			objHandler.PushObject(e);
		}
		
		void receive(const Events::NewObject_Event& entity);

		void start();

		static Level& getLevel() { return level; }

		ObjectHandler& getObjHendler() { return objHandler; }

		friend class Game;
	};

}

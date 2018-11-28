#pragma once
#include "DebuggingSystem.h"
#include "imgui.h"
#include "imgui-sfml.h"
#include "Actor.h"

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

		void handelEvent(sf::Event& e); //TODO: переместить в update
		template<class Obj>
		Obj& GetObjects(std::string NAME);
		void PushObject(Entity* obj);
		void UpdateObjects(float time);
		void CollisionUpdate()
		{
			for (iter = ObjectsArray.begin(); iter != ObjectsArray.end(); iter++)
			{
				(*iter)->CollisionUpdate(ObjectsArray);
			}
		}
		void RenderObjects(sf::RenderWindow &WINDOW);
		void refresh();
	};

	class World
	{
	private:
		sf::Clock         mainClock, deltaClock;
		sf::Sprite        LevelSprite;
		sf::Texture       LevelTexture;
		ObjectHandler     objHandler;
		Level             level;
		sf::RenderTexture renderTexture;
		DebuggingSystem debug;
		bool ShowOverlay = true;

		void handleEvent(sf::Event& event);
		void draw(sf::RenderWindow& window);
		void update(sf::RenderWindow& window, float time, sf::Event& event);
		void Init(sf::RenderWindow& window);

	public:
		World()
		{
			renderTexture.create(1920, 1080);
		}
		~World() = default;

		void pushEntity(Entity* e)
		{
			debug.pushRectangle(e->getDebugRect());
			objHandler.PushObject(e);
		}
		Level* getLevel() { return &level; }
		ObjectHandler& getObjHendler() { return objHandler; }
		friend class Game;
	};

}

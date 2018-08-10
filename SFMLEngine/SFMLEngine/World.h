#pragma once
#include "Object.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "DebuggingSystem.h"

namespace Engine
{
	enum appState
	{
		Pause = 0,
		Run,
		Stop,
		Err
	};

	class ObjectHandler
	{
	private:
		std::vector<Entity*> ObjectsArray;
		std::vector<Entity*>::iterator iter;
#if Debug
		Debugging::DebuggingSystem debug;
#endif
	public:
		ObjectHandler() = default;
		~ObjectHandler() = default;

		Entity& GetObjects(std::string NAME);
		void PushObject(Entity* obj);
		void UpdateObjects(float time);
		void RenderObjects(sf::RenderWindow &WINDOW);
		void refresh()
		{
			ObjectsArray.erase(std::remove_if(std::begin(ObjectsArray), std::end(ObjectsArray), [](const Entity *entity)
			{
				return !entity->isActive();
			}));
		}

	};

	class World
	{
	private:
		static float time;
		sf::Clock mainClock;
		sf::RenderWindow* window;
		ObjectHandler objHandler;
		appState state;

		void update();
		void handleEvent(sf::Event& event);
		void draw();
		void init();

	public:
		World(sf::RenderWindow& w) : window(&w) {};
		~World() { delete window; };

		void startGame();
		ObjectHandler& getObjHendler() { return objHandler; }
		static float getTime() { return time; }
	};
}

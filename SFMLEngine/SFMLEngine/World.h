#pragma once
#include "Object.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "DebuggingSystem.h"
#include "Level.h"
#if Debug 
#include "imgui.h"
#include "imgui-sfml.h"
#endif

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
	public:
		ObjectHandler() = default;
		~ObjectHandler() = default;
#if Debug
		DebuggingSystem debug;
#endif
		template<class Obj>
		Obj& GetObjects(std::string NAME);
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
		sf::Sprite LevelSprite;
		sf::Texture LevelTexture;
		ObjectHandler objHandler;
		appState state;
		Level level;
		sf::View view;
#if Debug 
		DebuggingSystem debug;
		bool ShowOverlay = true;
#endif
		void update();
		void handleEvent(sf::Event& event);
		void draw();
		void init();
		void pushEntity(Entity* e) 
		{
#if Debug
			debug.pushRectangle(e->getDebugRect());
#endif;
			objHandler.PushObject(e);
		}
	public:
		World(sf::RenderWindow& w) : window(&w) {};
		~World() { delete window; };

		void startGame();
		ObjectHandler& getObjHendler() { return objHandler; }
		static float getTime() { return time; }
	};

#if Debug
#endif
}

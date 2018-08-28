#pragma once
#include "DebuggingSystem.h"
#if Debug 
#include "imgui.h"
#include "imgui-sfml.h"
#endif

namespace Engine
{
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
		sf::Clock mainClock, deltaClock;
		sf::Sprite LevelSprite;
		sf::Texture LevelTexture;
		ObjectHandler objHandler;
		Level level;
		sf::View view;
#if Debug 
		DebuggingSystem debug;
		bool ShowOverlay = true;
#endif
		void pushEntity(Entity* e)
		{
#if Debug
			debug.pushRectangle(e->getDebugRect());
#endif
			objHandler.PushObject(e);
		}

		void handleEvent(sf::Event& event, sf::RenderWindow& window);
		void draw(sf::RenderWindow& window);
		void update(sf::RenderWindow& window, float time);
		void Init(sf::RenderWindow& window);

	public:
		World() = default;
		~World() = default;

		ObjectHandler& getObjHendler() { return objHandler; }
		friend class Game;
	};

}

#pragma once
#include "DebuggingSystem.h"
#if _Debug_ 
#include "imgui.h"
#include "imgui-sfml.h"
#endif
#include "Actor.h"

namespace Engine
{
	class ObjectHandler
	{
	private:
		std::vector<Entity*>           ObjectsArray;
		std::vector<Entity*>::iterator iter;
	public:
#if _Debug_
		DebuggingSystem debug;
#endif
		ObjectHandler() = default;
		~ObjectHandler()
		{
			for (iter = ObjectsArray.begin(); iter != ObjectsArray.end(); ++iter)
			{
				delete (*iter);
			}
			ObjectsArray.clear();
		}

		template<class Obj>
		Obj& GetObjects(std::string NAME);
		void PushObject(Entity* obj);
		void UpdateObjects(float time);
		void RenderObjects(sf::RenderWindow &WINDOW);
		void handelEvent(sf::Event& e) //Test
		{
			for (auto& o : ObjectsArray)
			{
				if (o->type == OActor)
					static_cast<Actor*>(o)->handleEvent(e);
			}
		}
		void refresh();
	};

	class World
	{
	private:
		sf::Clock     mainClock, deltaClock;
		sf::Sprite    LevelSprite;
		sf::Texture   LevelTexture;
		ObjectHandler objHandler;
		Level         level;
#if _Debug_  ///<warning The global flag. Debug = 1 when you need to debug

		DebuggingSystem debug;
		bool ShowOverlay = true;
#endif
		void pushEntity(Entity* e)
		{
#if _Debug_
			debug.pushRectangle(e->getDebugRect());
#endif
			objHandler.PushObject(e);
		}

		void handleEvent(sf::Event& event);
		void draw(sf::RenderWindow& window);
		void update(sf::RenderWindow& window, float time, sf::Event& event);
		void Init(sf::RenderWindow& window);

	public:
		World() = default;
		~World() = default;

		ObjectHandler& getObjHendler() { return objHandler; }
		friend class Game;
	};

}

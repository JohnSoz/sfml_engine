#pragma once
#include "DebuggingSystem.h"
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-sfml.h"
#include "Actor.h"
#include <entityx/entityx.h>
#include "Camera.h" 
#include "ObjectHandler.h"

namespace Engine
{
	class World : public entityx::Receiver<World>
	{
	private:
		sf::Clock         mainClock, deltaClock;
		sf::Sprite        LevelSprite;
		sf::Texture       LevelTexture;
		ObjectHandler     objHandler;
		Camera            cam;
		Level             level;
		DebuggingSystem   debug;
		bool              ShowOverlay;
	public:
		World();
		~World() = default;
		void Init(sf::RenderWindow& window);
		void handleEvent(sf::Event& event);
		void draw(sf::RenderWindow& window);
		void update(sf::RenderWindow& window, float time);
		void fixedUpdate();
		void load(sf::RenderWindow& window);
		template<class T>
		void receive(const Events::NewObject_Event<T>& entity)
		{
			entity.obj->setLevel(level);
			pushEntity(entity.obj);
		}
		void start(sf::RenderWindow& window);

		Level& getLevel() { return level; }
		Entity* pushEntity(Entity* e);
		ObjectHandler& getObjHendler() { return objHandler; }

		friend class Game;
	};

}

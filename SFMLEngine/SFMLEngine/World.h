#pragma once
#include "DebuggingSystem.h"
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-sfml.h"
#include "Actor.h"
#include <entityx/entityx.h>
#include "Camera.h" 

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
		~ObjectHandler();

		void handelEvent(sf::Event& e);
		void UpdateObjects(float time);
		void CollisionUpdate();	
		void RenderObjects(sf::RenderWindow& WINDOW);
		void refresh();

		template<class Obj>
		Obj* GetObjects(std::string NAME);
		Entity* PushObject(Entity* obj); 
	};

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
		void updateImGui();
		void load(sf::RenderWindow& window);
		void receive(const Events::NewObject_Event& entity);
		void start(sf::RenderWindow& window);

		Level& getLevel() { return level; }
		Entity* pushEntity(Entity* e);
		ObjectHandler& getObjHendler() { return objHandler; }

		friend class Game;
	};

}

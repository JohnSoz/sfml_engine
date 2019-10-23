#pragma once
#include <vector>
#include "Entity.h"
#include "DebuggingSystem.h"
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
		void fixedUpdate();
		void CollisionUpdate();
		void RenderObjects(sf::RenderWindow& WINDOW);
		bool hasObject(std::string_view name);

		template<class Obj>
		Obj* GetObjects(std::string NAME)
		{
			auto retObj = std::find_if(ObjectsArray.begin(), ObjectsArray.end(),
				[NAME](const Entity* e1)->bool
				{
					return (e1->getName() == NAME);
				});
			return static_cast<Obj*>(*retObj);
		}
		Entity* PushObject(Entity* obj);
	};
}
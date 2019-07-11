#pragma once
#include <entityx/Event.h>
#include <string>
#include "Entity.h"
namespace Engine
{
	struct EventManager
	{
		static entityx::EventManager eventManager;
	};
	namespace Events
	{
		struct Event_Inventory_UI : public entityx::Event<Event_Inventory_UI>
		{
			Event_Inventory_UI() {}
		};

		struct Main_Menu_Event
		{
			int state;
			Main_Menu_Event() {}
			Main_Menu_Event(int state) : state(state) {}
		};

		struct NewObject_Event
		{
			NewObject_Event(Entity& e) { obj = &e; }
			Entity* obj;
		};

		struct Change_State_Event
		{
			Change_State_Event(int stateId) : id(stateId) {}
			int id;
		};

	}
}


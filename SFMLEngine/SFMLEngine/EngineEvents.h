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
		template<class T>
		struct NewObject_Event
		{
			NewObject_Event(T& e) { obj = &e; }
			T* obj;
		};

		struct Change_State_Event
		{
			Change_State_Event(int stateId, bool changeWithLoadingScreen = false, int _action = 0) : id(stateId), _flag(changeWithLoadingScreen), action(_action) {}
			int id;
			int action;
			bool _flag;
		};

	}
}


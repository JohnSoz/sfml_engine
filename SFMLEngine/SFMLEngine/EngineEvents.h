#pragma once
#include <entityx/Event.h>
#include <string>

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

	}
}


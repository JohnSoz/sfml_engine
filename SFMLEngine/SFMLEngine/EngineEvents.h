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
		struct Event_UI : public entityx::Event<Event_UI>
		{
			Event_UI(std::string z) : msg(z) {}
			std::string msg;
		};
		struct EvSomeEvent : public entityx::Event<EvSomeEvent>
		{
			EvSomeEvent(int data) :some_data(data) {}
			int some_data;
		};
	}
}


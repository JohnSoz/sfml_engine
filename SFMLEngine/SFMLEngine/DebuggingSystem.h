#pragma once
/*!
\file
\brief Header file.
\warning Dependency on Object.h
*/
#include "Object.h"
#include "imgui.h"
#include "imgui-sfml.h"
#include "timer.h"
#define Debug 1; ///<warning The global flag. Debug = 1 when you need to debug

using namespace std;

namespace Engine
{
	struct ImGUI
	{
		static void ShowHelpMarker(const char* desc);
		static void SimpleOverlay(bool *open);
		static void SimpleText(sf::Vector2f position, bool *open, std::string name);

		///< log type
		enum logType
		{
			error = 1,
			info,
			fatal,
			system
		};

		/*!
		\brief Structure that stores the log data
		\bug
		\warning
		\example
		\todo
		*/
		struct Log
		{
			Log(std::string s, logType t);

			std::string              text; ///< Log text
			ImVec4                   color; ///< Color for log
			logType                  type; ///< Type log
		};

		/*!
		\brief The structure of which draws and adds the log
		\bug
		\warning
		\example
		\todo
		*/
		struct AppLog
		{
			bool                     ScrollToBottom; ///< Flag, true if the scrollbar
			std::vector<Log>         Buffer; ///< Ñontainer log, stores logs
			const char* items[5] = { "all","error", "info", "fatal", "system" }; ///< The text representation of the enumeration types of the log

			void Clear() { Buffer.clear(); Buffer.shrink_to_fit(); } ///< Clears the logs console \warning This method removes logs from the buffer

			void addLog(Log log); ///< Adds a log to the Buffer

			void Draw(const char* title, bool *p_open); ///< Draws the console logs \details ImGUI is used to draw the console
		};
	};

	class DebuggingSystem
	{
	public:
		static sf::RenderWindow* window;
		std::vector<std::pair<sf::FloatRect*, sf::FloatRect*>> entites;
		std::vector<sf::FloatRect> obj;
		bool overlay = false;
		bool LogConsole = false;
		sf::Clock Pressclock;
		static ImGUI::AppLog *log;
	public:
		DebuggingSystem() = default;
		~DebuggingSystem() = default;

		static void setWindow(sf::RenderWindow& w) { window = &w; }
		void draw();
		void pushRectangle(std::pair<sf::FloatRect*, sf::FloatRect*> e)
		{
			entites.push_back(e);
		}
		void levelObjects(std::vector<ObjectLevel> objs)
		{
			for (auto i : objs)
				obj.push_back(i.rect);
		}
		void handleEvent(sf::Event& event);
	};
}


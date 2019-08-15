#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
using namespace std;
namespace Engine 
{
	namespace Console
	{

		enum logType///< log type
		{
			error = 1,
			info,
			fatal,
			system,
			script,
			script_result
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
			std::string text; ///< Log text
			ImVec4 color;	 ///< Color for log
			logType type;	 ///< Type log
		};

		/*!
		\brief The structure of which draws and adds the log
		\bug
		\warning
		\example
		\todo
		*/

		class AppLog
		{
		private:
			static bool ScrollToBottom; ///< Flag, true if the scrollbar
			static std::vector<std::string> current_input;
			static vector<Log> Buffer;   ///< container log, stores logs
			static std::string items[7]; ///< The text representation of the enumeration types of the log

		public:
			AppLog() = default;
			~AppLog() { saveLog("Data/log.txt"); }
			static void Clear() ///< Clears the logs console \warning This method removes logs from the buffer
			{
				Buffer.clear();
				Buffer.shrink_to_fit();
			}

			static void addLog(Log log) { Buffer.push_back(log); } ///< Adds a log to the Buffer
			static void addLog(std::string s, logType t)
			{
				if (!Buffer.empty())
				{
					if (Buffer.back().text != s)
						Buffer.emplace_back(s, t);
				}
				else
					Buffer.emplace_back(s, t);
			}
			static void saveLog(std::string_view path);
			static void Draw(const char* title, bool* p_open); ///< Draws the console logs \details ImGUI is used to draw the console
		};
	} // namespace Console
}
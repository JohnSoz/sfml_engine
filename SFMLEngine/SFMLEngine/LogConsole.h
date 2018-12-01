#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include <windows.h>
#include <string>
#include <vector>
using namespace std;
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
namespace Console
{
	///< log type
	enum logType { error = 1, info, fatal, system, script , script_result};
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

	class AppLog
	{
	private:
		static bool        ScrollToBottom; ///< Flag, true if the scrollbar
		static std::vector<std::string> current_input;
		static vector<Log> Buffer; ///< Ñontainer log, stores logs
		static std::string items[7]; ///< The text representation of the enumeration types of the log
	public:
		static void Clear() { Buffer.clear(); Buffer.shrink_to_fit(); } ///< Clears the logs console \warning This method removes logs from the buffer

		static void addLog(Log log) { Buffer.push_back(log); } ///< Adds a log to the Buffer
		static void addLog(std::string s, logType t) { Buffer.emplace_back(Log(s, t)); }
		static void Draw(const char* title, bool *p_open, lua_State* state); ///< Draws the console logs \details ImGUI is used to draw the console
	};
}
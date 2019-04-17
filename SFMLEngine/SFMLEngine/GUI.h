#pragma once
#ifndef GUI_H
#define GUI_H

#include "Layer.h"
#include "LogConsole.h" 
#include <cassert>
#include "ApplicationState.h"
#include "LogConsole.h"
#include <vector>
#include <boost/type_index.hpp>
#include "json.hpp"
#include <fstream>
#include <iomanip>
using json = nlohmann::json;
using boost::typeindex::type_id_with_cvr;

namespace Engine
{
	class GuiEditor;
	class BaseGui
	{
	protected:
		GroupArray groupArray;
		std::string name;
		Gui gui;
		bool isEnable;
	public:
		virtual ~BaseGui() = default;
		BaseGui(sf::RenderWindow& w, std::string pathToTheme) : groupArray(w, pathToTheme) { isEnable = true; }

		virtual void draw() {}
		virtual void handleEvent(sf::Event&) {}
		bool IsEnable() const { return isEnable; }
		void activateOrDisable(const int n = -1);

		GroupArray& get_group_array() { return groupArray; }

		void save(std::string_view path);
		void load(std::string_view path);

		friend class GuiEditor;
	};
}

#endif
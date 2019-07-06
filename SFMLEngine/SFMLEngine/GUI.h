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
		bool isEnable; //Имеет смысл?
	public:
		BaseGui() = default;
		BaseGui(std::string_view path) { load(path); };
		BaseGui(sf::RenderWindow& w, std::string pathToTheme) : groupArray(w, pathToTheme) { isEnable = true; }
		virtual ~BaseGui() = default;
		virtual void draw() { gui.draw(); }
		virtual void update() {}
		virtual void handleEvent(sf::Event& e) { gui.handleEvent(e); }
		bool IsEnable() const { return isEnable; }
		void activateOrDisable(std::string_view name = "");

		GroupArray& get_group_array() { return groupArray; }

		void save(std::string_view path);
		void load(std::string_view path);

		friend class GuiEditor;
	};
}

#endif
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
using boost::typeindex::type_id_with_cvr;

namespace Engine
{
	class BaseGui
	{
	protected:
		GroupArray groupArray;
		Gui gui;
		bool isEnable;
	public:
		BaseGui() = delete;
		BaseGui(sf::RenderWindow& w, std::string pathToTheme) : groupArray(w, pathToTheme) { isEnable = true; }
		BaseGui(const BaseGui&) = delete;
		~BaseGui() = default;

		virtual void draw() = 0;
		virtual void handleEvent(sf::Event&) = 0;
		bool IsEnable() const { return isEnable; }
		void activateOrDisable(const int n = -1);
	};
}

#endif
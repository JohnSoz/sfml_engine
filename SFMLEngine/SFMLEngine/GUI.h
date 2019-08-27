#pragma once
#ifndef GUI_H
#define GUI_H
#include "GroupArray.h"

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
		BaseGui() = default;
		BaseGui(std::string_view path) { load(path); };
		BaseGui(sf::RenderWindow& w, std::string pathToTheme) : groupArray(w, pathToTheme) { isEnable = true; }
		virtual ~BaseGui() = default;

		virtual void draw() { gui.draw(); }
		virtual void update() = 0;
		virtual void handleEvent(sf::Event& e) { gui.handleEvent(e); }

		void activateOrDisable(std::string_view name = "");
		void disableAllExcept(std::string_view name);
		void save(std::string_view path);
		void load(std::string_view path);

		GroupArray& get_group_array() { return groupArray; }
		bool IsEnable() const { return isEnable; }
		
		friend class GuiEditor;
	};
}

#endif
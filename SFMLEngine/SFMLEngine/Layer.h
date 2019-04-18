#pragma once

#include "GuiUtility.h"

using namespace tgui;
using namespace sf;

namespace Engine
{
	class GUI;

	class GroupArray
	{
	private:
		std::vector<std::pair<std::string, Group::Ptr>> groups;
		sf::RenderWindow* windowPtr;
		tgui::Theme theme;
	public:
		GroupArray() = delete;
		GroupArray(sf::RenderWindow& w, std::string pathToTheme) : windowPtr(&w) { theme.load(pathToTheme); addGroup("Default"); }

		Group::Ptr get(std::string_view groupName);
		size_t size() const { return groups.size(); }
		auto begin() { return groups.begin(); }
		auto end() { return groups.end(); }

		Group::Ptr addGroup(std::string name) { groups.emplace_back(name, Group::create()); return (*(this->end() - 1)).second; }
		//void addWidget(std::pair<Widget::Ptr, std::string> widget, std::string wName, std::string_view groupName = "Default");
		void addWidget(Widget::Ptr wPtr, std::string wName, std::string_view groupName = "Default");


		Group::Ptr operator[](const int n) { return groups[n].second; }
	};
}
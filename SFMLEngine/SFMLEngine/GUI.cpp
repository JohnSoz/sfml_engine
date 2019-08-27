#include "LogConsole.h" 
#include <cassert>
#include "ApplicationState.h"
#include <vector>
#include <boost/type_index.hpp>
#include "json.hpp"
#include <fstream>
#include <iomanip>
#include "GUI.h"
using namespace Engine;
using json = nlohmann::json;
using boost::typeindex::type_id_with_cvr;

void Engine::BaseGui::activateOrDisable(std::string_view name /*= ""*/)
{
	Group::Ptr item;
	if (name.empty())
		item = groupArray[0];
	else
		item = std::find_if(groupArray.begin(), groupArray.end(),
			[name](std::pair<std::string, Group::Ptr>& item)
			{return item.first == name; })->second;

	const bool enabled = item->isEnabled();
	if (enabled)
	{
		item->setVisible(false);
		item->setEnabled(false);
	}
	else
	{
		item->setVisible(true);
		item->setEnabled(true);
	}
	isEnable = item->isEnabled(); //?
}

void Engine::BaseGui::disableAllExcept(std::string_view name)
{
	auto item = *std::find_if(groupArray.begin(), groupArray.end(),
		[name](std::pair<std::string, Group::Ptr>& item)
		{return item.first == name; });
	item.second->setVisible(true);
	item.second->setEnabled(true);
	for (auto& group : groupArray)
	{
		if (group.first != item.first)
		{
			group.second->setVisible(false);
			group.second->setEnabled(false);
		}
	}
}

void Engine::BaseGui::save(std::string_view path)
{
	std::ofstream o;
	o.open(path.data());
	json j;

	///j["GUI"] = name;
	///j["GUI"] = pathToTheme;

	for (auto group : groupArray)
	{
		j[group.first]["IsVisible"] = group.second->isVisible();
		for (const auto& w_ptr : group.second->getWidgets())
		{
			auto& widget = j[group.first][group.second->getWidgetName(w_ptr)];
			widget["Type"] = w_ptr->getWidgetType();
			widget["Position"]["x"] = w_ptr->getPosition().x;
			widget["Position"]["y"] = w_ptr->getPosition().y;
			widget["Size"]["x"] = w_ptr->getSize().x;
			widget["Size"]["y"] = w_ptr->getSize().y;
			widget["IsVisible"] = w_ptr->isVisible();
			if (group.second->getWidgetType() == "Picture")
			{

			}
		}
	}
	o << std::setw(4) << j;
	o.close();
}

void Engine::BaseGui::load(std::string_view path)
{
	std::ifstream o;
	o.open(path.data());
	json j;
	o >> j;
	for (const auto& item : j.items()) //Group Information
	{
		Console::AppLog::addLog("Group: " + item.key(), Console::info);
		auto group = this->get_group_array().addGroup(item.key());
		gui.add(group, item.key());
		if (item.value().is_object())
			for (const auto& item2 : item.value().items()) //Widget name
			{
				Console::AppLog::addLog("Widget: " + item2.key(), Console::info);

				auto Type = item2.value().at("Type").get<std::string>();
				auto Position = item2.value().at("Position");
				auto Size = item2.value().at("Size");

				auto posX = Position.at("x").get<float>();
				auto posY = Position.at("y").get<float>();

				auto sizeX = Size.at("x").get<float>();
				auto sizeY = Size.at("y").get<float>();

				auto isVisible = item2.value().at("IsVisible").get<bool>();

				Widget::Ptr widget;

				if (Type == "Button")
				{
					widget = makeButton(item2.key(), { posX, posY }, { sizeX, sizeX });
				}
				else if (Type == "Canvas")
				{
					//widget = makeCanvas(item2.key(), { posX, posY }, { sizeX, sizeX });
				}
				else if (Type == "TextBox")
				{
					widget = makeTextBox(item2.key(), { posX, posY }, { sizeX, sizeX });
				}
				else if (Type == "ListBox")
				{
					widget = makeListBox({ item2.key() }, { posX, posY }, { sizeX, sizeX });
				}
				widget->setVisible(isVisible);
				groupArray.addWidget(widget, item2.key(), item.key());
			}
	}
}

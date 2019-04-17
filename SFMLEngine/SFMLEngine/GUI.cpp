#include "GUI.h"
using namespace Engine;

void Engine::BaseGui::activateOrDisable(const int n /*= -1*/)
{
	const auto enabled = groupArray[0]->isEnabled();
	if (n == -1)
	{
		if (enabled)
		{
			groupArray[0]->setVisible(false);
			groupArray[0]->setEnabled(false);
		}
		else
		{
			groupArray[0]->setVisible(true);
			groupArray[0]->setEnabled(true);
		}
	}
	else
	{
		for (auto& [type, item] : groupArray)
		{
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
		}
	}
	isEnable = groupArray[0]->isEnabled();
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
	j << o;
	for (const auto& item : j.items()) //Group Information
	{
		Console::AppLog::addLog("Group: " + item.key(), Console::info);
		auto group = this->get_group_array().addGroup(item.key());
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

				tgui::Widget::Ptr widget;

				if (Type == "Button")
				{
					widget = tgui::Button::create();
				}
				else if ("Canvas")
				{
					widget = tgui::Canvas::create();
				}
				else if ("TextBox")
				{
					widget = tgui::TextBox::create();
				}
				else if ("ListBox")
				{
					widget = tgui::ListBox::create();
				}
				widget->setPosition(posX, posY);
				widget->setSize(sizeX, sizeX);
				widget->setVisible(isVisible);
				group->add(widget, item2.key());
			}
	}
}

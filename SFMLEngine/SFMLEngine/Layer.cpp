#include "Layer.h"
using namespace Engine;

//void Engine::GroupArray::addWidget(std::pair<Widget::Ptr, std::string> widget, std::string wName, std::string_view groupName /*= "Default"*/)
//{
//	auto group = std::find_if(groups.begin(), groups.end(), [groupName](const std::pair<std::string, Group::Ptr> & item)
//	{
//		return item.first == groupName;
//	});
//	widget.first->setRenderer(theme.getRenderer(widget.second));
//	if (group != groups.end())
//		group->second->add(widget.first, wName);
//}

tgui::Group::Ptr Engine::GroupArray::get(std::string_view groupName)
{
	auto group = std::find_if(groups.begin(), groups.end(), [groupName](const std::pair<std::string, Group::Ptr>& item)
		{
			return item.first == groupName;
		});
	return (group != groups.end()) ? group->second : nullptr;
}

void Engine::GroupArray::addWidget(Widget::Ptr wPtr, std::string wName, std::string_view groupName /*= "Default"*/)
{
	auto group = std::find_if(groups.begin(), groups.end(), [groupName](const std::pair<std::string, Group::Ptr>& item)
		{
			return item.first == groupName;
		});
	if (wPtr->getWidgetType() != "Canvas" && wPtr->getWidgetType() != "Picture" && wPtr->getWidgetType() != "Group")
		wPtr->setRenderer(theme.getRenderer(wPtr->getWidgetType()));
	if (group != groups.end())
		group->second->add(wPtr, wName);
}

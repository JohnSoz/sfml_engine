#pragma once

#include "GUI.h"
#include <fstream>
#include <iomanip>

namespace Engine
{
	class GuiEditor
	{
		BaseGui* ptrGui;

		enum MenuState { GROUPVIEW = 0, ADDWIDGET, ADDGROUP, SAVE, LOAD };
		std::string item_current;
		MenuState menuState = GROUPVIEW;
		char buffer_name[256];
		char buffer_path[256];

	public:
		bool isEnable = true;

		GuiEditor() = default;
		GuiEditor(BaseGui& b) : ptrGui(&b)
		{
			item_current = ptrGui->get_group_array()[0].first;
			buffer_name[0] = '\0';
			buffer_path[0] = '\0';
		}

		void load();
		void saveWindow();
		void groupViewWindow();
		void addWidgetWindow();
		void addGroupWindow();
		void drawEditor();
	};
}
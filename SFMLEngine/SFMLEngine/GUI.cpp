#include "GUI.h"
using namespace Engine;

void Engine::BaseGui::activateOrDisable(int n /*= -1*/)
{
	bool enabled = groupArray[0]->isEnabled();
	if (n = -1)
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

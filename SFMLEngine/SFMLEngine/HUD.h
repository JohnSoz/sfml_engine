#pragma once
#include "GUI.h"
#include "Inventory.h"
#include "GuiEditor.h"

namespace Engine
{
	class HUD : public BaseGui
	{
		Inventory* inv;
		GuiEditor gEditor;
	public:
		HUD() = default;
		HUD(std::string pathToTheme, sf::RenderWindow& w, Inventory& i);

		void makeHud();
		void update() override;
		void draw() override;
	};
}
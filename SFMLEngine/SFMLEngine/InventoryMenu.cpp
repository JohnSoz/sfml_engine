#include "InventoryMenu.h"
using namespace Engine;
void Engine::InventoryMenu::moveAll(sf::Vector2f offset)
{
	for (auto& w : groupArray[0]->getWidgets())
		w->setPosition((sf::Vector2f)w->getPosition() + offset);
}

#include "Inventory.h"
using namespace Engine;
std::string itemType_s[3] = { "item","gun","heal" };

std::string Engine::Item::getType_s()
{
	return itemType_s[type];
}

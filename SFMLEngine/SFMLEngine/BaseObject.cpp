#include "BaseObject.h"
#include <utility>
bool Engine::operator==(const BaseObject& obj, const BaseObject& obj2)
{
	return std::make_pair(obj.name, obj.type) == std::make_pair(obj2.name, obj2.type);
}

Engine::BaseObject::BaseObject(std::string_view _name) :
	name(_name), type(ObjectType::BObject), IsActive(true)
{

}

#include "Component.h"

Component::Component()
{
}

Component::~Component()
{
}

string Component::GetComponentName()
{
	return _componentName;
}

void Component::SetComponentName(string componentName)
{
	_componentName = componentName;
}

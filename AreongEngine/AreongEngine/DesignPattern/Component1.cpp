#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* gameObject) : gameObject(gameObject), transform(gameObject->transform)
{
}

Component::~Component()
{
}

void Component::setOwner(GameObject * pOwner)
{
}

GameObject * Component::GetOwner() const
{
	return m_pOwner;
}

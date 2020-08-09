#include "Collider.h"
#include "RigidbodyComponent.h"

Collider::Collider()
{
}

Collider::~Collider()
{
}

void Collider::UpdateComponent()
{
}

bool Collider::Init(GameObject * newGameObject)
{
	if (newGameObject == nullptr)
	{
		MessageBox(0, L"newGameObject == nullptr", L"Error", MB_OK);
		return false;
	}

	gameObject = newGameObject;

	rigidbody = gameObject->GetComponent<RigidbodyComponent>();
	if (rigidbody == nullptr)
	{
		MessageBox(0, L"Set rigidbody failed", L"Error", MB_OK);
		return false;
	}

	SetComponentName("Collider");

	return true;
}

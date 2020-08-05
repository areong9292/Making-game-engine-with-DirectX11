#include "Collider.h"

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
	gameObject = newGameObject;
}

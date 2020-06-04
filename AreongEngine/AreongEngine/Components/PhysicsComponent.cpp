#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
{
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::UpdateComponent()
{
	// 매 프레임마다 물리 작용
	// 중력을 어떻게 구현하면 좋을까?
	// 1. Transform의 포인터를 받아 아래로 누를까?
	// 2. 컴포넌트끼리 통신을 만들어서 할까?

	//cout << "PhysicsComponent Update" << endl;
}

bool PhysicsComponent::Init(Transform * transform)
{
	objTransform = transform;

	return true;
}

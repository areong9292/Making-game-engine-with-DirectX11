#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
{
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::UpdateComponent()
{
	// �� �����Ӹ��� ���� �ۿ�
	// �߷��� ��� �����ϸ� ������?
	// 1. Transform�� �����͸� �޾� �Ʒ��� ������?
	// 2. ������Ʈ���� ����� ���� �ұ�?

	//cout << "PhysicsComponent Update" << endl;
}

bool PhysicsComponent::Init(Transform * transform)
{
	objTransform = transform;

	return true;
}

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
	if (objTransform != nullptr)
	{
		objTransform->_position.y -= 0.18;
		if (objTransform->_position.y <= 0)
			objTransform->_position.y == 0;
	}
}

bool PhysicsComponent::Init(Transform * transform)
{
	objTransform = transform;

	return true;
}

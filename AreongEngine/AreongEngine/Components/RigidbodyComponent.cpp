#include "RigidbodyComponent.h"

RigidbodyComponent::RigidbodyComponent()
{
}

RigidbodyComponent::~RigidbodyComponent()
{
}

void RigidbodyComponent::UpdateComponent()
{
	// �� �����Ӹ��� ���� �ۿ�
	// �߷��� ��� �����ϸ� ������?
	// 1. Transform�� �����͸� �޾� �Ʒ��� ������?
	// 2. ������Ʈ���� ����� ���� �ұ�?

	//cout << "RigidbodyComponent Update" << endl;
	if (objTransform != nullptr)
	{
		objTransform->_position.y -= 0.18;
		if (objTransform->_position.y <= 0)
			objTransform->_position.y == 0;
	}
}

bool RigidbodyComponent::Init(Transform * transform)
{
	objTransform = transform;

	return true;
}

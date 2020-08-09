#include "RigidbodyComponent.h"
#include "Collider.h"
#include "../ColliderManager.h"

RigidbodyComponent::RigidbodyComponent()
{
	// ������ �ٵ�� �ݶ��̴� �Ŵ����� �����ϰ� �ִ´�
	m_colliderManager = Singleton<ColliderManager>::GetInstance();
}

RigidbodyComponent::RigidbodyComponent(Collider* collider)
{
	objCollider = collider;
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
		// ���� ���� ����?
		if (m_colliderManager != nullptr)
		{
			// ������ �ٵ� ���� �ݶ��̴��� �ٸ� �ݶ��̴���� �浹�� ���� ���
			if (m_colliderManager->CheckCollider(objCollider).size() == 0)
			{
				objTransform->_position.y -= 0.18;
				if (objTransform->_position.y <= 0)
					objTransform->_position.y == 0;
			}
		}
	}
}

bool RigidbodyComponent::Init(Transform * transform)
{
	objTransform = transform;

	return true;
}

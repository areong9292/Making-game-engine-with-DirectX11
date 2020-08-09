#include "RigidbodyComponent.h"
#include "Collider.h"
#include "../ColliderManager.h"

RigidbodyComponent::RigidbodyComponent()
{
	// 리지드 바디는 콜라이더 매니저를 참조하고 있는다
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
	// 매 프레임마다 물리 작용
	// 중력을 어떻게 구현하면 좋을까?
	// 1. Transform의 포인터를 받아 아래로 누를까?
	// 2. 컴포넌트끼리 통신을 만들어서 할까?

	//cout << "RigidbodyComponent Update" << endl;
	if (objTransform != nullptr)
	{
		// 물리 적용 가능?
		if (m_colliderManager != nullptr)
		{
			// 리지드 바디가 가진 콜라이더와 다른 콜라이더들과 충돌이 없는 경우
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

#pragma once
#include "../DesignPattern/Component.h"
#include "../DesignPattern/Transform.h"

class Collider;
class ColliderManager;
class RigidbodyComponent : public Component
{
public:
	RigidbodyComponent();
	RigidbodyComponent(Collider* collider);
	~RigidbodyComponent();

	// �� �����Ӹ��� ȣ��
	virtual void UpdateComponent() override;

	bool Init(Transform* transform);

private:
	Transform* objTransform = nullptr;
	Collider* objCollider = nullptr;

	// �浹 �����ϴ� �Ŵ���
	ColliderManager* m_colliderManager = nullptr;
};


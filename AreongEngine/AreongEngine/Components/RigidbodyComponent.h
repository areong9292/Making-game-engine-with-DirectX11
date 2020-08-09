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

	// 매 프레임마다 호출
	virtual void UpdateComponent() override;

	bool Init(Transform* transform);

private:
	Transform* objTransform = nullptr;
	Collider* objCollider = nullptr;

	// 충돌 관리하는 매니저
	ColliderManager* m_colliderManager = nullptr;
};


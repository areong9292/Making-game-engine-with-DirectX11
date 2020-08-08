#pragma once
#include "../DesignPattern/Component.h"
#include "../DesignPattern/Transform.h"

class RigidbodyComponent : public Component
{
public:
	RigidbodyComponent();
	~RigidbodyComponent();

	// �� �����Ӹ��� ȣ��
	virtual void UpdateComponent() override;

	bool Init(Transform* transform);

private:
	Transform* objTransform = nullptr;
};


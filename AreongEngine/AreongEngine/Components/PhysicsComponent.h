#pragma once
#include "../DesignPattern/Component.h"
#include "../DesignPattern/Transform.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent();
	~PhysicsComponent();

	// �� �����Ӹ��� ȣ��
	virtual void UpdateComponent() override;

	bool Init(Transform* transform);

private:
	Transform* objTransform = nullptr;
};


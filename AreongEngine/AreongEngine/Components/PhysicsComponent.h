#pragma once
#include "../DesignPattern/Component.h"
class PhysicsComponent : public Component
{
	PhysicsComponent();
	~PhysicsComponent();

	// �� �����Ӹ��� ȣ��
	virtual void UpdateComponent() override;
};


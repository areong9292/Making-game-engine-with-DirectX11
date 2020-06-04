#pragma once
#include "../DesignPattern/Component.h"
class PhysicsComponent : public Component
{
	PhysicsComponent();
	~PhysicsComponent();

	// 매 프레임마다 호출
	virtual void UpdateComponent() override;
};


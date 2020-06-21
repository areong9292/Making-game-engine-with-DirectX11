#pragma once
#include "../DesignPattern/Component.h"
#include "../DesignPattern/Transform.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent();
	~PhysicsComponent();

	// 매 프레임마다 호출
	virtual void UpdateComponent() override;

	bool Init(Transform* transform);

private:
	Transform* objTransform = nullptr;
};


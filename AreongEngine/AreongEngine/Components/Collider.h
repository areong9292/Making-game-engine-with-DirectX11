#pragma once
#include "../DesignPattern/Component.h"
#include "../DesignPattern/GameObject.h"
#include "PhysicsComponent.h"

// 충돌을 담당하는 컴포넌트
class Collider : public Component
{
public:
	Collider();
	~Collider();

	// 매 프레임마다 호출
	virtual void UpdateComponent() override;

	bool Init(GameObject* newGameObject);

private:
	// 이 컴포넌트를 소유하고 있는 GameObject
	GameObject* gameObject = nullptr;

	// 이 콜라이더가 가지는 물리
	PhysicsComponent* rigidbody = nullptr;
};


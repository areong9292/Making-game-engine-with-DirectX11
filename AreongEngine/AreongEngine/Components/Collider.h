#pragma once
#include "../DesignPattern/Component.h"
#include "../DesignPattern/GameObject.h"
#include "PhysicsComponent.h"

// �浹�� ����ϴ� ������Ʈ
class Collider : public Component
{
public:
	Collider();
	~Collider();

	// �� �����Ӹ��� ȣ��
	virtual void UpdateComponent() override;

	bool Init(GameObject* newGameObject);

private:
	// �� ������Ʈ�� �����ϰ� �ִ� GameObject
	GameObject* gameObject = nullptr;

	// �� �ݶ��̴��� ������ ����
	PhysicsComponent* rigidbody = nullptr;
};


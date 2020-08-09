#pragma once
#include "../DesignPattern/Component.h"
#include "../DesignPattern/GameObject.h"

class RigidbodyComponent;
// �浹�� ����ϴ� ������Ʈ
class Collider : public Component
{
public:
	Collider();
	~Collider();

	// �� �����Ӹ��� ȣ��
	virtual void UpdateComponent() override;

	virtual bool Init(GameObject* newGameObject);

private:
	// �� ������Ʈ�� �����ϰ� �ִ� GameObject
	GameObject* gameObject = nullptr;

	// �� �ݶ��̴��� ������ ����
	RigidbodyComponent* rigidbody = nullptr;
};
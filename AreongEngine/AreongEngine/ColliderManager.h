#pragma once
#include "./DesignPattern/DesignPatternIncludes.h"
#include "Components/Collider.h"

// ��� Collider�� �����ϴ� �Ŵ���
class ColliderManager : Singleton<ColliderManager>
{
public:
	ColliderManager();
	~ColliderManager();

	vector<Collider*> CheckCollider(Collider* collider);

private:
	// ��� �ݶ��̴��� ���Ϳ� ����ִ´�
	vector<Collider*> colliderList;
};


#include "stdafx.h"
#include "ColliderManager.h"

ColliderManager::ColliderManager()
{
}

ColliderManager::~ColliderManager()
{
}

vector<Collider*> ColliderManager::CheckCollider(Collider * collider)
{
	vector<Collider*> tmpColliderList;

	// ��� �ݶ��̴��� ���鼭 �浹�� �ְ� �ִ��� üũ
	for (int i = 0; i < (int)colliderList.size(); i++)
	{
		// �浹���� ��� ���Ϳ� �ִ´�
		if (true)
		{
			tmpColliderList.push_back(colliderList[i]);
		}
	}

	return tmpColliderList;
}


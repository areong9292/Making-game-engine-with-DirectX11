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

	// 모든 콜라이더를 돌면서 충돌한 애가 있는지 체크
	for (int i = 0; i < (int)colliderList.size(); i++)
	{
		// 충돌됬을 경우 벡터에 넣는다
		if (true)
		{
			tmpColliderList.push_back(colliderList[i]);
		}
	}

	return tmpColliderList;
}


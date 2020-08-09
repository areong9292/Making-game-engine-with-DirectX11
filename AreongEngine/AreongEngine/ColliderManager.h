#pragma once
#include "./DesignPattern/DesignPatternIncludes.h"
#include "Components/Collider.h"

// 모든 Collider를 관리하는 매니저
class ColliderManager : Singleton<ColliderManager>
{
public:
	ColliderManager();
	~ColliderManager();

	vector<Collider*> CheckCollider(Collider* collider);

private:
	// 모든 콜라이더를 벡터에 담고있는다
	vector<Collider*> colliderList;
};


#pragma once

#include "Component.h"
#include <vector>


class Transform : public Component
{
public:
	BEGIN_COMPONENT_BASE

	// 트렌스폼 최상단에 Root를 둔다
	// 모든 GameObject의 Transform은 Root자식(의 자식의....)으로 구성된다
	static Transform* Root;

	Transform(GameObject* gameObject);
	virtual ~Transform();

	virtual void onCreate();
	virtual void onDestroy();

	void addChild(Transform* child);
	void removeChild(Transform* child);
	Transform* getParent();

	vector<Transform*> getChildren();
	int getChildCount();
	Transform* getChild(int index);


private:
	virtual void UpdateComponent();

	Transform* _parent;
	vector<Transform*> _children;
};

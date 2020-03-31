#pragma once

#include "Component.h"
#include <vector>


class Transform : public Component
{
public:
	BEGIN_COMPONENT_BASE

	// Ʈ������ �ֻ�ܿ� Root�� �д�
	// ��� GameObject�� Transform�� Root�ڽ�(�� �ڽ���....)���� �����ȴ�
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

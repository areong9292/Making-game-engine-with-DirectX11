#include "GameObject.h"
#include "Transform.h"

BEGIN_COMPONENT_INIT(Transform);

Transform* Transform::Root = (new GameObject("Root"))->transform;

// Component�� �����ڸ� ��������� ȣ���ؾ��Ѵ�
// �ȱ׷��� �⺻ �����ڰ� ȣ��Ǵµ� ������ �Լ����� ���� �߻���
Transform::Transform(GameObject* gameObject) : Component(gameObject), _parent(Root)
{
	// ��Ʈ �ڽ����� �־��ش�
	if (Root != nullptr)
	{
		Root->_children.push_back(this);
	}
}

Transform::~Transform()
{
}

void Transform::onCreate()
{
}

void Transform::onDestroy()
{
}

Transform* Transform::getParent()
{
	return _parent;
}

void Transform::addChild(Transform * child)
{
	// ���� ������ �θ�κ��� ����
	child->_parent->removeChild(child);

	// ��⼭ ���� ��� ��ǥ, ������ ó���� �ʿ��ϴ�
	// ���� ����غ���

	// �� �� �θ� ����
	child->_parent = this;

	// �ڽ� ����Ʈ�� �ִ´�
	_children.push_back(child);
}

void Transform::removeChild(Transform * child)
{
	// �ڽ� ����Ʈ ��ȸ�Ͽ� ���� �Ÿ� ����Ʈ���� �����Ѵ�
	for (auto it = _children.begin(); it != _children.end(); ++it)
	{
		if (*it == child)
		{
			_children.erase(it);

			// ������ �ڽ��� ��Ʈ�� �޾���
			child->_parent = Root;
			return;
		}
	}
}

vector<Transform*> Transform::getChildren()
{
	return _children;
}

int Transform::getChildCount()
{
	return (int)_children.size();
}

Transform * Transform::getChild(int index)
{
	return _children[index];
}

void Transform::UpdateComponent()
{
}
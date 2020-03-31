#include "Component.h"
#include "GameObject.h"

GameObject::GameObject(): transform(new Transform(this)), _name("GameObject")
{
	_components.push_back(transform);
}

GameObject::GameObject(const char* name) : transform(new Transform(this)), _name(name)
{
	_components.push_back(transform);
}

GameObject::GameObject(string name) : transform(new Transform(this)), _name(name.c_str())
{
	_components.push_back(transform);
}

GameObject::~GameObject()
{
	deleteTransforms();
	releaseComponents();
}

list<Component*> GameObject::getComponents()
{
	return _components;
}

const char * GameObject::getName()
{
	return _name;
}

void GameObject::deleteTransforms()
{
	// ��� Ʈ������ �ڽ��� ��ȸ�Ͽ� �����Ѵ�
	for (auto child : transform->getChildren())
	{
		child->gameObject->deleteTransforms();
	}

	// �θ�κ��� ������ Ʈ���������� ������ش�
	if (transform->getParent())
	{
		transform->getParent()->removeChild(transform);
	}
}

void GameObject::releaseComponents()
{
	for (auto component : _components)
	{
		component->onDestroy();
	}

	for (auto component : _components)
	{
		delete component;
	}
}
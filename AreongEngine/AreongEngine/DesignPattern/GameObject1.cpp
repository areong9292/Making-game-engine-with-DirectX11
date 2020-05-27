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
	// 모든 트랜스폼 자식을 순회하여 정리한다
	for (auto child : transform->getChildren())
	{
		child->gameObject->deleteTransforms();
	}

	// 부모로부터 별개의 트랜스폼으로 만들어준다
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
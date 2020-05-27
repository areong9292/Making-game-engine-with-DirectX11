#pragma once

#include <list>
#include <string>

#include "Transform.h"
using namespace std;

class Component;
class GameObject
{
public:
	GameObject();
	GameObject(const char* name);
	GameObject(string name);
	~GameObject();

	template <typename T> T* getComponent();
	template <typename T> T* addComponent();

	Transform* const transform;

	list<Component*> getComponents();

	const char* getName();

private:
	void deleteTransforms();
	void releaseComponents();

private:
	list<Component*> _components;
	const char* _name;
};

template <typename T> T* GameObject::getComponent()
{
	// ������Ʈ ����Ʈ�� ���鼭 ���� Ÿ���� ������Ʈ�� ã�´�
	for (auto component : _components)
	{
		// dynamic_cast
		// ��Ӱ��踦 �� ������ ���� Ÿ���� �ִ� ���
		// ������Ʈ Ÿ�Կ��� T Ÿ������ �����Ѵ�
		// ������ nullptr�� ������ �ȴ�
		T* target = dynamic_cast<T*>(component);
		if (target != nullptr)
			return target;
	}

	return nullptr;
}

template <typename T> T* GameObject::addComponent()
{
	// ������Ʈ ��Ӱ��谡 �ƴҰ�� false ������
	if (!is_base_of<Component, T>::value)
		return nullptr;

	// ��Ӱ��谡 ���� ��� ���ο� ������Ʈ ��ü ���� ��
	// �ʱ�ȭ �۾� �� ����Ʈ�� �־��ش�
	T* component = new T(this);
	component->onCreate();
	_components.push_back(component);

	return component;
}

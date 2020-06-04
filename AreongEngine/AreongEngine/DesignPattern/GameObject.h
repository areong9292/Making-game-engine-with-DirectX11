#pragma once
#include "../stdafx.h"
#include "Component.h"
#include "Transform.h"

class GameObject
{
public:
	GameObject();
	GameObject(string name);
	~GameObject();

	void SetName(string name);

	template <typename T> T* GetComponent();
	template <typename T> T* AddComponent();

	int GetComponentCount();

	// �� ������Ʈ�� �̸�
	string _name;

	Transform* _transform = nullptr;

private:
	vector<Component*> _listComponent;
};

template<typename T>
inline T * GameObject::GetComponent()
{
	for (int i = 0; i < (int)_listComponent.size(); i++)
	{
		// dynamic_cast
		// ��Ӱ��踦 �� ������ ���� Ÿ���� �ִ� ���
		// ������Ʈ Ÿ�Կ��� T Ÿ������ �����Ѵ�
		// ������ nullptr�� �����Ѵ�
		T* target = dynamic_cast<T*>(_listComponent[i]);
		if (target != nullptr)
			return target;
	}

	return nullptr;
}

template<typename T>
inline T * GameObject::AddComponent()
{
	// ������Ʈ ��Ӱ��谡 �ƴҰ�� nullptr ������
	if (!is_base_of<Component, T>::value)
		return nullptr;

	// ��Ӱ��谡 ���� ��� ���ο� ������Ʈ ��ü ���� ��
	// �ʱ�ȭ �۾� �� ����Ʈ�� �־��ش�
	T* component = new T();
	_listComponent.push_back(component);

	return component;
}

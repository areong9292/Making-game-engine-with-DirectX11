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

	// 이 오브젝트의 이름
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
		// 상속관계를 다 뒤져서 같은 타입이 있는 경우
		// 컴포넌트 타입에서 T 타입으로 변경한다
		// 없으면 nullptr을 리턴한다
		T* target = dynamic_cast<T*>(_listComponent[i]);
		if (target != nullptr)
			return target;
	}

	return nullptr;
}

template<typename T>
inline T * GameObject::AddComponent()
{
	// 컴포넌트 상속관계가 아닐경우 nullptr 리턴함
	if (!is_base_of<Component, T>::value)
		return nullptr;

	// 상속관계가 맞을 경우 새로운 컴포넌트 객체 생성 후
	// 초기화 작업 후 리스트에 넣어준다
	T* component = new T();
	_listComponent.push_back(component);

	return component;
}

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
	// 컴포넌트 리스트를 돌면서 같은 타입의 컴포넌트를 찾는다
	for (auto component : _components)
	{
		// dynamic_cast
		// 상속관계를 다 뒤져서 같은 타입이 있는 경우
		// 컴포넌트 타입에서 T 타입으로 변경한다
		// 없으면 nullptr을 가지게 된다
		T* target = dynamic_cast<T*>(component);
		if (target != nullptr)
			return target;
	}

	return nullptr;
}

template <typename T> T* GameObject::addComponent()
{
	// 컴포넌트 상속관계가 아닐경우 false 리턴함
	if (!is_base_of<Component, T>::value)
		return nullptr;

	// 상속관계가 맞을 경우 새로운 컴포넌트 객체 생성 후
	// 초기화 작업 후 리스트에 넣어준다
	T* component = new T(this);
	component->onCreate();
	_components.push_back(component);

	return component;
}

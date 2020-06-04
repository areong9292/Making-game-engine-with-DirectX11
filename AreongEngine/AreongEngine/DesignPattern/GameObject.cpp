#include "GameObject.h"

GameObject::GameObject()
{
	_transform = new Transform();
	_name = "GameObject";

	_listComponent.push_back(_transform);
}

GameObject::GameObject(string name)
{
	_transform = new Transform();
	_name = name;

	_listComponent.push_back(_transform);
}

GameObject::~GameObject()
{
	// 모든 컴포넌트를 해제한다
	for (int i = 0; i < (int)_listComponent.size(); i++)
	{
		delete _listComponent[i];
	}
}

// 매 프레임마다 호출
void GameObject::UpdateComponents()
{
	for (int i = 0; i < (int)_listComponent.size(); i++)
	{
		_listComponent[i]->UpdateComponent();
	}
}

void GameObject::SetName(string name)
{
	_name = name;
}

int GameObject::GetComponentCount()
{
	return _listComponent.size();
}

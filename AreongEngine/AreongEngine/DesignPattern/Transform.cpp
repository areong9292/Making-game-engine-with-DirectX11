#include "GameObject.h"
#include "Transform.h"

BEGIN_COMPONENT_INIT(Transform);

Transform* Transform::Root = (new GameObject("Root"))->transform;

// Component의 생성자를 명시적으로 호출해야한다
// 안그러면 기본 생성자가 호출되는데 삭제한 함수여서 에러 발생함
Transform::Transform(GameObject* gameObject) : Component(gameObject), _parent(Root)
{
	// 루트 자식으로 넣어준다
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
	// 먼저 기존의 부모로부터 제거
	child->_parent->removeChild(child);

	// 요기서 이제 상대 좌표, 스케일 처리가 필요하다
	// 좀더 고민해볼것

	// 그 후 부모 지정
	child->_parent = this;

	// 자식 리스트에 넣는다
	_children.push_back(child);
}

void Transform::removeChild(Transform * child)
{
	// 자식 리스트 순회하여 같은 거를 리스트에서 제거한다
	for (auto it = _children.begin(); it != _children.end(); ++it)
	{
		if (*it == child)
		{
			_children.erase(it);

			// 제거한 자식은 루트에 달아줌
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
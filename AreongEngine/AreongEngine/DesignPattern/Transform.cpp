#include "Transform.h"

Transform::Transform()
{
	// 컴포넌트 이름 셋팅
	SetComponentName("Transform");

	_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_scale = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

Transform::~Transform()
{
}

void Transform::UpdateComponent()
{
}


#include "Transform.h"

Transform::Transform()
{
	// 컴포넌트 이름 셋팅
	SetComponentName("Transform");

	_position = XMFLOAT3(0.0f, 100.0f, 0.0f);
	_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

Transform::~Transform()
{
}

// 매 프레임마다 호출
void Transform::UpdateComponent()
{
	//cout << "Transform Update" << endl;
}

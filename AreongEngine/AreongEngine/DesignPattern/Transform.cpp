#include "Transform.h"

Transform::Transform()
{
	// ������Ʈ �̸� ����
	SetComponentName("Transform");

	_position = XMFLOAT3(0.0f, 100.0f, 0.0f);
	_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

Transform::~Transform()
{
}

// �� �����Ӹ��� ȣ��
void Transform::UpdateComponent()
{
	//cout << "Transform Update" << endl;
}

#include "stdafx.h"
#include "CameraClass.h"

CameraClass::CameraClass()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return m_position;
}

XMFLOAT3 CameraClass::GetRotation()
{
	return m_rotation;
}

void CameraClass::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;

	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// ������ �ʱ�ȭ
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	// ī�޶� ��ġ ����
	position = m_position;

	positionVector = XMLoadFloat3(&position);

	// ���⺤�� �ʱ�ȭ
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	// ���� ȸ�� ������ yaw (Y��), pitch (X��), roll (Z��) ȸ���� �������� ����
	pitch = m_rotation.x * 0.0174532925f;
	yaw = m_rotation.y * 0.0174532925f;
	roll = m_rotation.z * 0.0174532925f;

	// ���� ������ ������ ȸ�� ����� ����
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// ���� ���Ϳ� �����͸� ȸ����ȯ �Ѵ�
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// ȸ���� ī�޶� ��ġ�� �� ��ġ�� �̵�
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// �� ����� �����
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void CameraClass::GetViewMatrix(XMMATRIX & viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

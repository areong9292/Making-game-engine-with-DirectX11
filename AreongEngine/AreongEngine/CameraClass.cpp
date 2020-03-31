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

	// 업벡터 초기화
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	// 카메라 위치 갱신
	position = m_position;

	positionVector = XMLoadFloat3(&position);

	// 방향벡터 초기화
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	// 현재 회전 값으로 yaw (Y축), pitch (X축), roll (Z축) 회전을 라디안으로 설정
	pitch = m_rotation.x * 0.0174532925f;
	yaw = m_rotation.y * 0.0174532925f;
	roll = m_rotation.z * 0.0174532925f;

	// 라디안 설정된 값으로 회전 행렬을 생성
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 방향 벡터와 업벡터를 회전변환 한다
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// 회전된 카메라 위치를 현 위치로 이동
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// 뷰 행렬을 만든다
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void CameraClass::GetViewMatrix(XMMATRIX & viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

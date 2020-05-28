#pragma once
#include "../stdafx.h"
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	~Transform();

	XMFLOAT3 _position;
	XMFLOAT3 _rotation;
	XMFLOAT3 _scale;

	// 매 프레임마다 호출
	virtual void UpdateComponent() override;

};


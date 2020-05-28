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
	
	virtual void UpdateComponent() override;

};


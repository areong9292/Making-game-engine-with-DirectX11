#pragma once
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	~Transform();

	virtual void UpdateComponent() override;

};


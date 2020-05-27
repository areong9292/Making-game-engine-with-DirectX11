#pragma once
#include "../stdafx.h"

class Component
{
public:
	Component();
	~Component();

	string GetComponentName();
	void SetComponentName(string componentName);

	virtual void UpdateComponent() = 0;

private:
	string _componentName;
};

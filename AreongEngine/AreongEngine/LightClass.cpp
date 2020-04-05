#include"stdafx.h"
#include "LightClass.h"

LightClass::LightClass()
{
}

LightClass::~LightClass()
{
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffseColor = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return m_diffseColor;
}

XMFLOAT3 LightClass::GetDirection()
{
	return m_direction;
}

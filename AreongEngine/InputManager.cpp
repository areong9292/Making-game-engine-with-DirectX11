#include "stdafx.h"
#include "InputManager.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

bool InputManager::Init()
{
	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return true;
}

void InputManager::KeyDown(unsigned int input)
{
	m_keys[input] = true;
}

void InputManager::KeyUp(unsigned int input)
{
	m_keys[input] = false;
}

bool InputManager::IsKeyDown(unsigned int input)
{
	return m_keys[input];
}

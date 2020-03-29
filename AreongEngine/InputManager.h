#pragma once
class InputManager
{
public:
	InputManager();
	~InputManager();

	bool Init();

	void KeyDown(unsigned int input);
	void KeyUp(unsigned int input);

	bool IsKeyDown(unsigned int input);

private:
	bool m_keys[256];
};


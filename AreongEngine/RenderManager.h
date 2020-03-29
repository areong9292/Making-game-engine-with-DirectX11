#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class D3DClass;
class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	bool Init(int screenWidth, int screenHeight, HWND hWnd);
	void Render();

	// DirectX Å¬·¡½º
	D3DClass* m_d3dClass = nullptr;
};
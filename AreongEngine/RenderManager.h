#pragma once
#include "D3DClass.h"

class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	bool Init(int screenWidth, int screenHeight, HWND hWnd);
	void Render();

	// DirectX Ŭ����
	D3DClass* m_d3dClass;
};

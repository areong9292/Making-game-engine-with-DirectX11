#pragma once
#include "D3DClass.h"

class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	bool InitializeRenderManager();
	void Render();

	// DirectX Ŭ����
	D3DClass* m_d3dClass;
};

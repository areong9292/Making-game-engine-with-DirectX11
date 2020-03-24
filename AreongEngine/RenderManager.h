#pragma once
#include "D3DClass.h"

class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	bool InitializeRenderManager();
	void Render();

	// DirectX Å¬·¡½º
	D3DClass* m_d3dClass;
};

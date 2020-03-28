#include "stdafx.h"
#include "RenderManager.h"

RenderManager::RenderManager()
{
	MessageBox(0, L"RenderManager", L"Error", MB_OK);
}

RenderManager::~RenderManager()
{
	if(m_d3dClass != nullptr)
		delete m_d3dClass;

	MessageBox(0, L"~RenderManager", L"Error", MB_OK);
}

// 렌더 매니저 초기화 메소드
bool RenderManager::InitializeRenderManager(int screenWidth, int screenHeight, HWND hWnd)
{
	m_d3dClass = new D3DClass();
	if (m_d3dClass != nullptr)
	{
		// DirectX11 초기화
		if (!m_d3dClass->InitializeD3DClass(screenWidth, screenHeight, hWnd))
		{
			MessageBox(0, L"Direct3D Initialization - Failed",
				L"Error", MB_OK);
			return false;
		}

		// d3d 씬 초기화
		if (!m_d3dClass->InitScene())
		{
			MessageBox(0, L"Scene Initialization - Failed",
				L"Error", MB_OK);
			return false;
		}
	}

	return true;
}

void RenderManager::Render()
{
	// DirectX로 매 프레임마다 컬러 값 변경 후 그린다
	if (m_d3dClass != nullptr)
	{
		m_d3dClass->UpdateScene();

		m_d3dClass->DrawScene();
	}
}

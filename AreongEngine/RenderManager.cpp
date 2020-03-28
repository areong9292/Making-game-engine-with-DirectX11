#include "stdafx.h"
#include "RenderManager.h"

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
	if(m_d3dClass != nullptr)
		delete m_d3dClass;
}

bool RenderManager::Init(int screenWidth, int screenHeight, HWND hWnd)
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
	if (m_d3dClass != nullptr)
	{
		m_d3dClass->UpdateScene();
		m_d3dClass->DrawScene();
	}
}

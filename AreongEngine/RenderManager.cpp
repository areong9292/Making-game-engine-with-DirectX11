#include "stdafx.h"
#include "RenderManager.h"
#include "D3DClass.h"

RenderManager::RenderManager()
{
	MessageBox(0, L"RenderManager", L"Error", MB_OK);
}

RenderManager::~RenderManager()
{
	if (m_d3dClass != nullptr)
		_aligned_free(m_d3dClass);

	MessageBox(0, L"~RenderManager", L"Error", MB_OK);
}

// 렌더 매니저 초기화 메소드
bool RenderManager::Init(int screenWidth, int screenHeight, HWND hWnd)
{
	m_d3dClass = (D3DClass*)_aligned_malloc(sizeof(D3DClass), 16);
	if (m_d3dClass != nullptr)
	{
		// DirectX11 초기화
		if (!m_d3dClass->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
		{
			MessageBox(0, L"Direct3D Initialization - Failed",
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
		// 씬을 그리기 위해 화면을 지운다
		m_d3dClass->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

		// 버퍼의 내용을 화면에 출력한다
		m_d3dClass->EndScene();
	}
}

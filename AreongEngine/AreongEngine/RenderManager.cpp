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

// ���� �Ŵ��� �ʱ�ȭ �޼ҵ�
bool RenderManager::Init(int screenWidth, int screenHeight, HWND hWnd)
{
	// DirectX11 �ʱ�ȭ
	m_d3dClass = new D3DClass();
	if (m_d3dClass != nullptr)
	{
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
		// ���� �׸��� ���� ȭ���� �����
		m_d3dClass->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

		// �׸����� �ϴ� ������ ���⼭ ó���Ѵ�

		// ������ ������ ȭ�鿡 ����Ѵ�
		m_d3dClass->EndScene();
	}
}

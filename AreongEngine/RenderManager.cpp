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

// ���� �Ŵ��� �ʱ�ȭ �޼ҵ�
bool RenderManager::InitializeRenderManager(int screenWidth, int screenHeight, HWND hWnd)
{
	m_d3dClass = new D3DClass();
	if (m_d3dClass != nullptr)
	{
		// DirectX11 �ʱ�ȭ
		if (!m_d3dClass->InitializeD3DClass(screenWidth, screenHeight, hWnd))
		{
			MessageBox(0, L"Direct3D Initialization - Failed",
				L"Error", MB_OK);
			return false;
		}

		// d3d �� �ʱ�ȭ
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
	// DirectX�� �� �����Ӹ��� �÷� �� ���� �� �׸���
	if (m_d3dClass != nullptr)
	{
		m_d3dClass->UpdateScene();

		m_d3dClass->DrawScene();
	}
}

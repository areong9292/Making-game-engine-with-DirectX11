#include "stdafx.h"
#include "RenderManager.h"

RenderManager::RenderManager()
{
	m_d3dClass = nullptr;

	MessageBox(0, L"RenderManager", L"Error", MB_OK);
}

RenderManager::~RenderManager()
{
	// �Ҵ��ߴ� ���ҽ��� �ʱ�ȭ
	if (m_d3dClass != nullptr)
	{
		delete m_d3dClass;
	}

	MessageBox(0, L"~RenderManager", L"Error", MB_OK);
}

// RenderManager �ʱ�ȭ �ϴ� �޼ҵ�
bool RenderManager::InitializeRenderManager()
{
	// DirectX Ŭ���� �ʱ�ȭ
	m_d3dClass = new D3DClass();
	if (m_d3dClass == nullptr)
		return false;

	if (!m_d3dClass->InitializeD3DClass())
		return false;

	return true;
}

// �������ϴ� �޼ҵ�
void RenderManager::Render()
{
	//MessageBox(0, L"RenderTest", L"Error", MB_OK);
}

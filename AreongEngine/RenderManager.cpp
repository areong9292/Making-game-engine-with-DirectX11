#include "stdafx.h"
#include "RenderManager.h"

RenderManager::RenderManager()
{
	m_d3dClass = nullptr;

	MessageBox(0, L"RenderManager", L"Error", MB_OK);
}

RenderManager::~RenderManager()
{
	// 할당했던 리소스들 초기화
	if (m_d3dClass != nullptr)
	{
		delete m_d3dClass;
	}

	MessageBox(0, L"~RenderManager", L"Error", MB_OK);
}

// RenderManager 초기화 하는 메소드
bool RenderManager::InitializeRenderManager()
{
	// DirectX 클래스 초기화
	m_d3dClass = new D3DClass();
	if (m_d3dClass == nullptr)
		return false;

	if (!m_d3dClass->InitializeD3DClass())
		return false;

	return true;
}

// 렌더링하는 메소드
void RenderManager::Render()
{
	//MessageBox(0, L"RenderTest", L"Error", MB_OK);
}

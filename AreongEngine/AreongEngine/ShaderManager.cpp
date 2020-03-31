#include "stdafx.h"
#include "ShaderManager.h"


ShaderManager::ShaderManager()
{
	m_colorShaderClass = nullptr;
}

ShaderManager::~ShaderManager()
{
	if (m_colorShaderClass != nullptr)
	{
		delete m_colorShaderClass;
		m_colorShaderClass = nullptr;
	}
}

bool ShaderManager::Initialize(ID3D11Device* device, HWND hWnd)
{
	// 필요한 쉐이더들 초기화
	m_colorShaderClass = new ColorShaderClass();
	if (m_colorShaderClass == nullptr)
	{
		MessageBox(0, L"Make ColorShaderClass Object - Failed",
			L"Error", MB_OK);
		return false;
	}

	if (!m_colorShaderClass->Initialize(device, hWnd))
	{
		MessageBox(0, L"ColorShaderClass Initialize - Failed",
			L"Error", MB_OK);
		return false;
	}

	return true;
}

bool ShaderManager::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	if (!m_colorShaderClass->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix))
	{
		return false;
	}

	return true;
}

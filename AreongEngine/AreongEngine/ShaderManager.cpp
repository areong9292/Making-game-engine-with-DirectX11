#include "stdafx.h"
#include "ShaderManager.h"


ShaderManager::ShaderManager()
{
	m_colorShaderClass = nullptr;
}

// �Ҵ��ߴ� ���̴��� ����
ShaderManager::~ShaderManager()
{
	if (m_colorShaderClass != nullptr)
	{
		delete m_colorShaderClass;
		m_colorShaderClass = nullptr;
	}

	if (m_textureShaderClass != nullptr)
	{
		delete m_textureShaderClass;
		m_textureShaderClass = nullptr;
	}

	MessageBox(0, L"~ShaderManager", L"Error", MB_OK);
}

// �ʿ��� ���̴��� �ʱ�ȭ
bool ShaderManager::Initialize(ID3D11Device* device, HWND hWnd)
{
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

	m_textureShaderClass = new TextureShaderClass();
	if (m_textureShaderClass == nullptr)
	{
		MessageBox(0, L"Make TextureShaderClass Object - Failed",
			L"Error", MB_OK);
		return false;
	}

	if (!m_textureShaderClass->Initialize(device, hWnd))
	{
		MessageBox(0, L"m_textureShaderClass Initialize - Failed",
			L"Error", MB_OK);
		return false;
	}

	return true;
}

// ���̴� Ÿ�Կ� �°� �׸���
bool ShaderManager::Render(ID3D11DeviceContext* deviceContext, int indexCount, ShaderType shaderType, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	switch (shaderType)
	{
	case ShaderType::ColorShader:
		if (!m_colorShaderClass->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix))
		{
			MessageBox(0, L"ColorShaderClass Render - Failed",
				L"Error", MB_OK);
			return false;
		}
		break;
	case ShaderType::TextureShader:
		if (!m_textureShaderClass->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture))
		{
			MessageBox(0, L"ColorShaderClass Render - Failed",
				L"Error", MB_OK);
			return false;
		}
		break;
	case ShaderType::LightShader:
		break;
	}
	return true;
}

#include "stdafx.h"
#include "RenderManager.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ShaderManager.h"

RenderManager::RenderManager()
{
	MessageBox(0, L"RenderManager", L"Error", MB_OK);
}

RenderManager::~RenderManager()
{
	if (m_model != nullptr)
	{
		delete m_model;
		m_model = nullptr;
	}

	if (m_camera != nullptr)
	{
		delete m_camera;
		m_camera = nullptr;
	}

	if (m_d3dClass != nullptr)
	{
		delete m_d3dClass;
		m_d3dClass = nullptr;
	}

	MessageBox(0, L"~RenderManager", L"Error", MB_OK);
}

// ���� �Ŵ��� �ʱ�ȭ �޼ҵ�
bool RenderManager::Init(int screenWidth, int screenHeight, HWND hWnd)
{
	// DirectX11 �ʱ�ȭ
	m_d3dClass = new D3DClass();

	if (m_d3dClass == nullptr)
	{
		MessageBox(0, L"Make Direct3D Object - Failed",
			L"Error", MB_OK);
	}

	if (!m_d3dClass->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(0, L"Direct3D Initialization - Failed",
			L"Error", MB_OK);
		return false;
	}

	// ī�޶� Ŭ���� �ʱ�ȭ
	m_camera = new CameraClass();
	if (m_camera == nullptr)
	{
		MessageBox(0, L"Make Camera Object - Failed",
			L"Error", MB_OK);
	}

	m_camera->SetPosition(0.0f, 0.0f, -5.0f);

	// �� Ŭ���� �ʱ�ȭ
	m_model = new ModelClass();
	if (m_model == nullptr)
	{
		MessageBox(0, L"Make Camera Object - Failed",
			L"Error", MB_OK);
	}
	if (!m_model->Initialize(m_d3dClass->GetDevice(), L"./Data/seafloor.dds"))
	{
		MessageBox(0, L"ModelClass Initialization - Failed",
			L"Error", MB_OK);
	}

	// ���̴� 
	m_shaderManager = Singleton<ShaderManager>::GetInstance();
	if (m_shaderManager == nullptr)
	{
		MessageBox(0, L"Make Direct3D Object - Failed",
			L"Error", MB_OK);
	}

	if (!m_shaderManager->Initialize(m_d3dClass->GetDevice(), hWnd))
	{
		MessageBox(0, L"ShaderManager Initialization - Failed",
			L"Error", MB_OK);
		return false;
	}

	return true;
}

bool RenderManager::Render()
{
	if (m_d3dClass != nullptr)
	{
		// ���� �׸��� ���� ȭ���� �����
		m_d3dClass->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

		// �׸����� �ϴ� ������ ���⼭ ó���Ѵ�
		m_camera->Render();

		// ����, ��, ���� ��� �����´�
		XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
		m_d3dClass->GetWorldMatrix(worldMatrix);
		m_camera->GetViewMatrix(viewMatrix);
		m_d3dClass->GetProjectionMatrix(projectionMatrix);

		// ���� ���ؽ�, �ε��� ���۸� �׷��� ����������(�Է� �����)�� �����Ͽ� �׸��⸦ �غ��Ѵ�
		m_model->Render(m_d3dClass->GetDeviceContext());

		if (!m_shaderManager->Render(m_d3dClass->GetDeviceContext(), m_model->GetIndexCount(), m_model->GetShaderType(), worldMatrix, viewMatrix, projectionMatrix, m_model->GetTexture()))
		{
			MessageBox(0, L"ShaderManager Render - Failed",
				L"Error", MB_OK);

			return false;
		}

		// ������ ������ ȭ�鿡 ����Ѵ�
		m_d3dClass->EndScene();

	}
	return true;
}

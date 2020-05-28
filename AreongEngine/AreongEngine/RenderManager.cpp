#include "stdafx.h"
#include "RenderManager.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "LightClass.h"
#include "ShaderManager.h"

RenderManager::RenderManager()
{
	MessageBox(0, L"RenderManager", L"Error", MB_OK);
}

RenderManager::~RenderManager()
{
	if (testObj != nullptr)
	{
		delete testObj;
		testObj = nullptr;
	}

	if (m_light != nullptr)
	{
		delete m_light;
		m_light = nullptr;
	}

	ModelClass* temp;
	for (int i = 0; i < (int)modelList.size(); i++)
	{
		temp = modelList[i];
		if (temp != nullptr)
		{
			delete temp;
			temp = nullptr;
		}
	}

	/*
	if (m_model != nullptr)
	{
		delete m_model;
		m_model = nullptr;
	}
	*/

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

		return false;
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

		return false;
	}

	m_camera->SetPosition(0.0f, 100.0f, -300.0f);

	// �� Ŭ���� �ʱ�ȭ
	m_model = new ModelClass();
	if (m_model == nullptr)
	{
		MessageBox(0, L"Make ModelClass Object - Failed",
			L"Error", MB_OK);

		return false;
	}
	if (!m_model->Initialize(m_d3dClass->GetDevice(), L"./Resources/Textures/seafloor.dds", L"./Resources/Models/Parsed/scrat"))
	{
		MessageBox(0, L"ModelClass Initialization - Failed",
			L"Error", MB_OK);

		return false;
	}

	modelList.push_back(m_model);

	// ����Ʈ Ŭ���� �ʱ�ȭ
	m_light = new LightClass();
	if (m_light == nullptr)
	{
		MessageBox(0, L"Make LightClass Object - Failed",
			L"Error", MB_OK);

		return false;
	}

	// ����Ʈ ��ü ����
	m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetDirection(0.0f, 0.0f, 1.0f);

	// ���̴� 
	m_shaderManager = Singleton<ShaderManager>::GetInstance();
	if (m_shaderManager == nullptr)
	{
		MessageBox(0, L"Make Direct3D Object - Failed",
			L"Error", MB_OK);

		return false;
	}

	if (!m_shaderManager->Initialize(m_d3dClass->GetDevice(), hWnd))
	{
		MessageBox(0, L"ShaderManager Initialization - Failed",
			L"Error", MB_OK);
		return false;
	}

	testObj = new GameObject("1234");
	if (testObj == nullptr)
	{
		MessageBox(0, L"GameObject Initialization - Failed",
			L"Error", MB_OK);
		return false;
	}

	return true;
}

bool RenderManager::Render()
{
	if (m_d3dClass != nullptr)
	{
	//	static float rotation = 0.0f;

		rotation += 3.14f * 0.01f;
		if (rotation > 360.0f)
		{
			rotation = 0.0f;
		}

		// ���� �׸��� ���� ȭ���� �����
		m_d3dClass->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

		// �׸����� �ϴ� ������ ���⼭ ó���Ѵ�
		m_camera->Render();

		// ����, ��, ���� ��� �����´�
		//XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
		m_d3dClass->GetWorldMatrix(worldMatrix);
		m_camera->GetViewMatrix(viewMatrix);
		m_d3dClass->GetProjectionMatrix(projectionMatrix);

		rotationMatrix = worldMatrix;
		translateMatrix = worldMatrix;

		// ȸ�� ����
		//worldMatrix = XMMatrixRotationY(rotation);

		for (int i = 0; i < (int)modelList.size(); i++)
		{
			modelPosition = modelList[i]->GetPosition();

			// Y�� �������� ȸ����Ų��
			rotationMatrix = XMMatrixRotationY(rotation);

			// ���� ��ġ�� ���� ��� �̵�
			translateMatrix = XMMatrixTranslation(modelPosition.x, modelPosition.y, modelPosition.z);

			// ���� ���� ��� ��� - ������ * ȸ�� * �̵� ����� ���ؼ� ���ϴ� ����� ��´�
			resultMatrix = /*scaleMatrix **/ rotationMatrix * translateMatrix;

			// ���� ���ؽ�, �ε��� ���۸� �׷��� ����������(�Է� �����)�� �����Ͽ� �׸��⸦ �غ��Ѵ�
			modelList[i]->Render(m_d3dClass->GetDeviceContext());

			if (!m_shaderManager->Render(m_d3dClass->GetDeviceContext(), modelList[i]->GetIndexCount(), modelList[i]->GetShaderType(),
				resultMatrix, viewMatrix, projectionMatrix,
				modelList[i]->GetTexture(), m_light->GetDirection(), modelList[i]->GetDiffuseColor()))//m_light->GetDiffuseColor()))
			{
				MessageBox(0, L"ShaderManager Render - Failed",
					L"Error", MB_OK);

				return false;
			}

			rotationMatrix = worldMatrix;
			translateMatrix = worldMatrix;
		}
		
		// ������ ������ ȭ�鿡 ����Ѵ�
		m_d3dClass->EndScene();
	}
	return true;
}

void RenderManager::MakeCube()
{
	// �� Ŭ���� �ʱ�ȭ
	ModelClass* temp = new ModelClass();
	if (temp == nullptr)
	{
		MessageBox(0, L"Make ModelClass Object - Failed",
			L"Error", MB_OK);
		
		return;
	}
	if (!temp->Initialize(m_d3dClass->GetDevice(), L"./Resources/Textures/seafloor.dds", L"./Resources/Models/Parsed/Cube"))
	{
		MessageBox(0, L"ModelClass Initialization - Failed",
			L"Error", MB_OK);

		return;
	}

	// ������ ��ġ�� ��ǥ ����
	// �õ� ���� ��� ���� random_device ����
	random_device rd;

	// random_device�� ���� ���� ���� ������ �ʱ�ȭ
	mt19937 gen(rd());

	// -5.0 ���� 5.0���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����
	uniform_real_distribution<float> dis(-5.0f, 5.0f);

	temp->SetPosition(dis(gen), dis(gen), 0.0f);

	modelList.push_back(temp);

	// ������Ʈ���� Transform ������Ʈ ������
	testTransform = testObj->GetComponent<Transform>();
	if (testTransform != nullptr)
	{
		cout << testTransform->GetComponentName() << endl;
	}
}

// �� ����Ʈ���� ���� ���� �ϳ��� �����Ѵ�
void RenderManager::SelectPlayer()
{
	_selectIndex++;

	if (_selectIndex >= (int)modelList.size())
	{
		_selectIndex = 0;
	}

	if (m_selectedModel != nullptr)
		m_selectedModel->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

	m_selectedModel = modelList[_selectIndex];
	m_selectedModel->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);

}

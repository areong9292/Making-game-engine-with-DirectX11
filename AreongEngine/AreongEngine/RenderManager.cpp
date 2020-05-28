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

// 렌더 매니저 초기화 메소드
bool RenderManager::Init(int screenWidth, int screenHeight, HWND hWnd)
{
	// DirectX11 초기화
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

	// 카메라 클래스 초기화
	m_camera = new CameraClass();
	if (m_camera == nullptr)
	{
		MessageBox(0, L"Make Camera Object - Failed",
			L"Error", MB_OK);

		return false;
	}

	m_camera->SetPosition(0.0f, 100.0f, -300.0f);

	// 모델 클래스 초기화
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

	// 라이트 클래스 초기화
	m_light = new LightClass();
	if (m_light == nullptr)
	{
		MessageBox(0, L"Make LightClass Object - Failed",
			L"Error", MB_OK);

		return false;
	}

	// 라이트 객체 셋팅
	m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetDirection(0.0f, 0.0f, 1.0f);

	// 쉐이더 
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

		// 씬을 그리기 위해 화면을 지운다
		m_d3dClass->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

		// 그리고자 하는 동작을 여기서 처리한다
		m_camera->Render();

		// 월드, 뷰, 투영 행렬 가져온다
		//XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
		m_d3dClass->GetWorldMatrix(worldMatrix);
		m_camera->GetViewMatrix(viewMatrix);
		m_d3dClass->GetProjectionMatrix(projectionMatrix);

		rotationMatrix = worldMatrix;
		translateMatrix = worldMatrix;

		// 회전 적용
		//worldMatrix = XMMatrixRotationY(rotation);

		for (int i = 0; i < (int)modelList.size(); i++)
		{
			modelPosition = modelList[i]->GetPosition();

			// Y축 기준으로 회전시킨다
			rotationMatrix = XMMatrixRotationY(rotation);

			// 모델의 위치로 월드 행렬 이동
			translateMatrix = XMMatrixTranslation(modelPosition.x, modelPosition.y, modelPosition.z);

			// 최종 월드 행렬 계산 - 스케일 * 회전 * 이동 행렬을 곱해서 원하는 결과를 얻는다
			resultMatrix = /*scaleMatrix **/ rotationMatrix * translateMatrix;

			// 모델의 버텍스, 인덱스 버퍼를 그래픽 파이프라인(입력 어셈블러)에 전달하여 그리기를 준비한다
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
		
		// 버퍼의 내용을 화면에 출력한다
		m_d3dClass->EndScene();
	}
	return true;
}

void RenderManager::MakeCube()
{
	// 모델 클래스 초기화
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

	// 랜덤한 위치로 좌표 지정
	// 시드 값을 얻기 위한 random_device 생성
	random_device rd;

	// random_device를 통해 난수 생성 엔진을 초기화
	mt19937 gen(rd());

	// -5.0 부터 5.0까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의
	uniform_real_distribution<float> dis(-5.0f, 5.0f);

	temp->SetPosition(dis(gen), dis(gen), 0.0f);

	modelList.push_back(temp);

	// 오브젝트에서 Transform 컴포넌트 가져옴
	testTransform = testObj->GetComponent<Transform>();
	if (testTransform != nullptr)
	{
		cout << testTransform->GetComponentName() << endl;
	}
}

// 모델 리스트에서 다음 모델을 하나를 선택한다
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

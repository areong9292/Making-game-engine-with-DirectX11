#include "stdafx.h"
#include "TextureClass.h"
#include "ModelDataParserClass.h"

#include "ModelClass.h"

ModelClass::ModelClass()
{
	SetPosition(0.0, 0.0f, 0.0f);
	SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 파일 확장자 셋팅
	SetExtension(L".obj");

	// 파싱 된 파일 확장자 셋팅
	parsedModelExtension = L".txt";
	//wcscpy_s(parsedModelExtension, wcslen(L".txt"), L".txt");

	modelParser = new ModelDataParserClass();
	if (modelParser == nullptr)
	{
		MessageBox(0, L"modelParser Init fail", L"Error", MB_OK);
	}
}

ModelClass::~ModelClass()
{
	if (modelParser != nullptr)
	{
		delete modelParser;
		modelParser = nullptr;
	}

	if (m_model != nullptr)
	{
		delete[] m_model;
		m_model = nullptr;
	}
	// 텍스처 반환
	if (m_texture != nullptr)
	{
		delete m_texture;
		m_texture = nullptr;
	}
	// 버텍스, 인덱스 버퍼 반환
	if (m_indexBuffer != nullptr)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	if (m_vertexBuffer != nullptr)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

bool ModelClass::Initialize(ID3D11Device* device, WCHAR* textureFilename, WCHAR* modelFileName)
{
	if (!InitializeBuffers(device, modelFileName))
	{
		return false;
	}

	return LoadTexture(device, textureFilename);
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView * ModelClass::GetTexture()
{
	if (m_texture == nullptr)
		return nullptr;

	return m_texture->GetTexture();
}

void ModelClass::SetPosition(float x, float y, float z)
{
	modelPosition.x = x;
	modelPosition.y = y;
	modelPosition.z = z;
}

XMFLOAT3 ModelClass::GetPosition()
{
	return modelPosition;
}

void ModelClass::SetDiffuseColor(float x, float y, float z, float w)
{
	modelDiffustColor.x = x;
	modelDiffustColor.y = y;
	modelDiffustColor.z = z;
	modelDiffustColor.w = w;
}

XMFLOAT4 ModelClass::GetDiffuseColor()
{
	return modelDiffustColor;
}

void ModelClass::SetExtension(WCHAR * extension)
{
	// 파일 확장자 셋팅
	modelExtension = extension;
	//wcscpy_s(modelExtension, wcslen(extension), extension);
}

ShaderManager::ShaderType ModelClass::GetShaderType()
{
	return m_shaderType;
}

bool ModelClass::CheckAndModelLoader(WCHAR * modelFileName)
{
	// 파일 있는 지 여부 체크
	struct stat buffer;

	WCHAR txtFilePath[100];
	::ZeroMemory(txtFilePath, sizeof(txtFilePath));

	WCHAR modelFilePath[100];
	::ZeroMemory(modelFilePath, sizeof(modelFilePath));

	//wmemset(modelFilePath, 0, 100);

	//txtFilePath = modelFileName;
	wcscpy_s(txtFilePath, 100, modelFileName);
	wcscat_s(txtFilePath, 100, parsedModelExtension);

	//modelFilePath = modelFileName;
	wcscpy_s(modelFilePath, 100, modelFileName);
	wcscat_s(modelFilePath, 100, modelExtension);

	_bstr_t existCheck(txtFilePath);

	// 해당 파일이 존재하는 경우 모델 로드 진행
	if (stat(existCheck, &buffer) == 0)
	{
		if (!ModelLoader(txtFilePath))
		{
			MessageBox(0, L"Model File Load fail", L"Error", MB_OK);
			return false;
		}
	}

	// 없는 경우 파일 파싱 후 모델 로드 진행
	else
	{
		if (modelParser != nullptr)
		{
			modelParser->ProcessParser(modelFilePath, txtFilePath, modelExtension);

			_bstr_t existCheck(txtFilePath);

			// 해당 파일이 존재하는 경우 모델 로드 진행
			if (stat(existCheck, &buffer) == 0)
			{
				if (!ModelLoader(txtFilePath))
				{
					MessageBox(0, L"Model File Load fail", L"Error", MB_OK);
					return false;
				}
			}
		}
	}

	return true;
}

bool ModelClass::ModelLoader(WCHAR * modelFileName)
{
	// 파일 읽기
	ifstream readFile(modelFileName);
	string tmp;
	int i = 0;
	if (readFile.is_open())
	{
		// data까지 읽음
		readFile >> tmp >> m_vertexCount >> tmp;
		m_indexCount = m_vertexCount;

		// 모델정보 초기화 및 파일 읽기
		m_model = new ModelType[m_vertexCount];

		while (!readFile.eof())
		{
			// 파일 읽기
			readFile >> m_model[i].x >> m_model[i].y >> m_model[i].z;
			readFile >> m_model[i].tu >> m_model[i].tv;
			readFile >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;

			i++;
		}

		// 정점 배열을 만든다
		vertices = new VertexType[m_vertexCount];
		if (vertices == nullptr)
		{
			MessageBox(0, L"Make vertex array fail", L"Error", MB_OK);
			return false;
		}

		// 인덱스 배열을 만든다
		indices = new unsigned long[m_indexCount];
		if (indices == nullptr)
		{
			MessageBox(0, L"Make index array fail", L"Error", MB_OK);
			return false;
		}

		// 모델 정보를 버텍스, 인덱스 배열에 채운다
		for (i = 0; i < m_vertexCount; i++)
		{
			vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
			vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
			vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

			indices[i] = i;
		}
	}
	else
	{
		readFile.close();
		return false;
	}

	readFile.close();
	return true;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device, WCHAR* modelFileName)
{
	// 파일 정보가 있는 경우 그 정보로 버텍스, 인덱스 정보를 채운다
	if (modelFileName != L"")
	{
		if (!CheckAndModelLoader(modelFileName))
		{
			MessageBox(0, L"CheckAndModelLoader fail", L"Error", MB_OK);
			return false;
		}
	}

	// 아니면 삼각형으로 그린다
	else
	{
		// 정점, 인덱스 수를 설정한다
		m_vertexCount = 3;
		m_indexCount = 3;

		// 정점 배열을 만든다
		vertices = new VertexType[m_vertexCount];
		if (vertices == nullptr)
		{
			MessageBox(0, L"Make vertex array fail", L"Error", MB_OK);
			return false;
		}

		// 인덱스 배열을 만든다
		indices = new unsigned long[m_indexCount];
		if (indices == nullptr)
		{
			MessageBox(0, L"Make index array fail", L"Error", MB_OK);
			return false;
		}

		// 임시로 삼각형 하드 코딩
		// 정점 데이터 설정
		vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
		vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

		vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertices[1].texture = XMFLOAT2(0.5f, 0.0f);

		vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
		vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

		// 인덱스 배열 설정
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
	}
	
	// 정보가 제대로 안 갖춰진 경우 리턴
	if (vertices == nullptr || indices == nullptr)
	{
		MessageBox(0, L"Make vertex | index array fail", L"Error", MB_OK);
		return false;
	}

	// 각 버텍스의 정보로 버텍스 버퍼, 인덱스 버퍼를 만든다

	// 버텍스 버퍼 설정
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 버텍스 버퍼 실제 데이터 포인터 연결
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		MessageBox(0, L"Make vertex buffer fail", L"Error", MB_OK);
		return false;
	}

	// 버텍스 버퍼 설정
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 버퍼 실제 데이터 포인터 연결
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		MessageBox(0, L"Make index buffer fail", L"Error", MB_OK);
		return false;
	}

	// 정점, 인덱스 배열 해제
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// 버텍스 버퍼 의 단위와 오프셋 설정
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// 렌더링을 위해 앞서 셋팅했던 버텍스, 인덱스 버퍼를 입력 어셈블러에 전달하는 과정

	// 입력 어셈블러에 버텍스 퍼버 활성으로 설정
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// 입력 어셈블러에 인덱스 퍼버 활성으로 설정
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 그리기 기본형을 삼각형으로 설정
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR * textureFilename)
{
	m_texture = new TextureClass();
	if (m_texture == nullptr)
	{
		return false;
	}

	return m_texture->Initialize(device, textureFilename);
}
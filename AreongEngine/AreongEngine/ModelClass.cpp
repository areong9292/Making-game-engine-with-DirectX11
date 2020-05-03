#include "stdafx.h"
#include "TextureClass.h"
#include "ModelDataParserClass.h"

#include "ModelClass.h"

ModelClass::ModelClass()
{
	SetPosition(0.0, 0.0f, 0.0f);
	SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

	// ���� Ȯ���� ����
	SetExtension(L".obj");

	// �Ľ� �� ���� Ȯ���� ����
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
	// �ؽ�ó ��ȯ
	if (m_texture != nullptr)
	{
		delete m_texture;
		m_texture = nullptr;
	}
	// ���ؽ�, �ε��� ���� ��ȯ
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
	// ���� Ȯ���� ����
	modelExtension = extension;
	//wcscpy_s(modelExtension, wcslen(extension), extension);
}

ShaderManager::ShaderType ModelClass::GetShaderType()
{
	return m_shaderType;
}

bool ModelClass::CheckAndModelLoader(WCHAR * modelFileName)
{
	// ���� �ִ� �� ���� üũ
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

	// �ش� ������ �����ϴ� ��� �� �ε� ����
	if (stat(existCheck, &buffer) == 0)
	{
		if (!ModelLoader(txtFilePath))
		{
			MessageBox(0, L"Model File Load fail", L"Error", MB_OK);
			return false;
		}
	}

	// ���� ��� ���� �Ľ� �� �� �ε� ����
	else
	{
		if (modelParser != nullptr)
		{
			modelParser->ProcessParser(modelFilePath, txtFilePath, modelExtension);

			_bstr_t existCheck(txtFilePath);

			// �ش� ������ �����ϴ� ��� �� �ε� ����
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
	// ���� �б�
	ifstream readFile(modelFileName);
	string tmp;
	int i = 0;
	if (readFile.is_open())
	{
		// data���� ����
		readFile >> tmp >> m_vertexCount >> tmp;
		m_indexCount = m_vertexCount;

		// ������ �ʱ�ȭ �� ���� �б�
		m_model = new ModelType[m_vertexCount];

		while (!readFile.eof())
		{
			// ���� �б�
			readFile >> m_model[i].x >> m_model[i].y >> m_model[i].z;
			readFile >> m_model[i].tu >> m_model[i].tv;
			readFile >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;

			i++;
		}

		// ���� �迭�� �����
		vertices = new VertexType[m_vertexCount];
		if (vertices == nullptr)
		{
			MessageBox(0, L"Make vertex array fail", L"Error", MB_OK);
			return false;
		}

		// �ε��� �迭�� �����
		indices = new unsigned long[m_indexCount];
		if (indices == nullptr)
		{
			MessageBox(0, L"Make index array fail", L"Error", MB_OK);
			return false;
		}

		// �� ������ ���ؽ�, �ε��� �迭�� ä���
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
	// ���� ������ �ִ� ��� �� ������ ���ؽ�, �ε��� ������ ä���
	if (modelFileName != L"")
	{
		if (!CheckAndModelLoader(modelFileName))
		{
			MessageBox(0, L"CheckAndModelLoader fail", L"Error", MB_OK);
			return false;
		}
	}

	// �ƴϸ� �ﰢ������ �׸���
	else
	{
		// ����, �ε��� ���� �����Ѵ�
		m_vertexCount = 3;
		m_indexCount = 3;

		// ���� �迭�� �����
		vertices = new VertexType[m_vertexCount];
		if (vertices == nullptr)
		{
			MessageBox(0, L"Make vertex array fail", L"Error", MB_OK);
			return false;
		}

		// �ε��� �迭�� �����
		indices = new unsigned long[m_indexCount];
		if (indices == nullptr)
		{
			MessageBox(0, L"Make index array fail", L"Error", MB_OK);
			return false;
		}

		// �ӽ÷� �ﰢ�� �ϵ� �ڵ�
		// ���� ������ ����
		vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
		vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

		vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertices[1].texture = XMFLOAT2(0.5f, 0.0f);

		vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
		vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

		// �ε��� �迭 ����
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
	}
	
	// ������ ����� �� ������ ��� ����
	if (vertices == nullptr || indices == nullptr)
	{
		MessageBox(0, L"Make vertex | index array fail", L"Error", MB_OK);
		return false;
	}

	// �� ���ؽ��� ������ ���ؽ� ����, �ε��� ���۸� �����

	// ���ؽ� ���� ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// ���ؽ� ���� ���� ������ ������ ����
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		MessageBox(0, L"Make vertex buffer fail", L"Error", MB_OK);
		return false;
	}

	// ���ؽ� ���� ����
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// �ε��� ���� ���� ������ ������ ����
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		MessageBox(0, L"Make index buffer fail", L"Error", MB_OK);
		return false;
	}

	// ����, �ε��� �迭 ����
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// ���ؽ� ���� �� ������ ������ ����
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// �������� ���� �ռ� �����ߴ� ���ؽ�, �ε��� ���۸� �Է� ������� �����ϴ� ����

	// �Է� ������� ���ؽ� �۹� Ȱ������ ����
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// �Է� ������� �ε��� �۹� Ȱ������ ����
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �׸��� �⺻���� �ﰢ������ ����
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
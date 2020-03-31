#include "stdafx.h"
#include "ModelClass.h"

ModelClass::ModelClass()
{
}

ModelClass::~ModelClass()
{
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

bool ModelClass::Initialize(ID3D11Device * device)
{
	return InitializeBuffers(device);
}

void ModelClass::Render(ID3D11DeviceContext * deviceContext)
{
	RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

bool ModelClass::InitializeBuffers(ID3D11Device * device)
{
	// ����, �ε��� ���� �����Ѵ�
	m_vertexCount = 3;
	m_indexCount = 3;

	// ���� �迭�� �����
	VertexType* vertices = new VertexType[m_vertexCount];
	if (vertices == nullptr)
	{
		MessageBox(0, L"Make vertex array fail", L"Error", MB_OK);
		return false;
	}

	// �ε��� �迭�� �����
	unsigned long* indices = new unsigned long[m_indexCount];
	if (indices == nullptr)
	{
		MessageBox(0, L"Make index array fail", L"Error", MB_OK);
		return false;
	}

	// �ӽ÷� �ﰢ�� �ϵ� �ڵ�
	// ���� ������ ����
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// �ε��� �迭 ����
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

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

void ModelClass::RenderBuffers(ID3D11DeviceContext * deviceContext)
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

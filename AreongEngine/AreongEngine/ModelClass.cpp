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
	// 정점, 인덱스 수를 설정한다
	m_vertexCount = 3;
	m_indexCount = 3;

	// 정점 배열을 만든다
	VertexType* vertices = new VertexType[m_vertexCount];
	if (vertices == nullptr)
	{
		MessageBox(0, L"Make vertex array fail", L"Error", MB_OK);
		return false;
	}

	// 인덱스 배열을 만든다
	unsigned long* indices = new unsigned long[m_indexCount];
	if (indices == nullptr)
	{
		MessageBox(0, L"Make index array fail", L"Error", MB_OK);
		return false;
	}

	// 임시로 삼각형 하드 코딩
	// 정점 데이터 설정
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// 인덱스 배열 설정
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

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

void ModelClass::RenderBuffers(ID3D11DeviceContext * deviceContext)
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

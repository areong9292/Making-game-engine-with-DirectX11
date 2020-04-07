#pragma once

#include "ShaderManager.h"
class TextureClass;
class ModelClass : public AlignedAllocationPolicy<16>
{
private:
	// 정점 버퍼에 사용할 정점의 구조체 선언
	// 버텍스 쉐이더의 입력과 형태가 같아야한다
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;	// 텍스쳐를 입힐 것이다
		XMFLOAT3 normal;	// 법선 벡터
	};

	struct ModelType
	{
		float x, y, z;		// 모델의 위치
		float tu, tv;		// 텍스쳐를 입힐 것이다
		float nx, ny, nz;	// 법선 벡터
	};

public:
	ModelClass();
	~ModelClass();

	bool Initialize(ID3D11Device* device, WCHAR* textureFilename, WCHAR* modelFileName = L"");
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

	ShaderManager::ShaderType GetShaderType();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool ModelLoader(WCHAR* modelFileName);
	bool InitializeBuffers(ID3D11Device* device, WCHAR* modelFileName = L"");
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, WCHAR* textureFilename);

private:
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

	VertexType* vertices = nullptr;
	unsigned long* indices = nullptr;

	int m_vertexCount = 0;
	int m_indexCount = 0;

	ShaderManager::ShaderType m_shaderType = ShaderManager::ShaderType::LightShader;

	TextureClass* m_texture = nullptr;

	ModelType* m_model = nullptr;
};

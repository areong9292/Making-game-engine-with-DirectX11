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
	};

public:
	ModelClass();
	~ModelClass();

	bool Initialize(ID3D11Device* device, WCHAR* textureFilename);
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

	ShaderManager::ShaderType GetShaderType();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, WCHAR* textureFilename);

private:
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

	int m_vertexCount = 0;
	int m_indexCount = 0;

	ShaderManager::ShaderType m_shaderType = ShaderManager::ShaderType::TextureShader;

	TextureClass* m_texture = nullptr;
};

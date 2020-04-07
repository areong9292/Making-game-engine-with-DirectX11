#pragma once

#include "ShaderManager.h"
class TextureClass;
class ModelClass : public AlignedAllocationPolicy<16>
{
private:
	// ���� ���ۿ� ����� ������ ����ü ����
	// ���ؽ� ���̴��� �Է°� ���°� ���ƾ��Ѵ�
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;	// �ؽ��ĸ� ���� ���̴�
		XMFLOAT3 normal;	// ���� ����
	};

	struct ModelType
	{
		float x, y, z;		// ���� ��ġ
		float tu, tv;		// �ؽ��ĸ� ���� ���̴�
		float nx, ny, nz;	// ���� ����
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

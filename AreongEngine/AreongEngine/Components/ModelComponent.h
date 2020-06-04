#pragma once

#include "../ShaderManager.h"

#include "../stdafx.h"
#include "../TextureClass.h"
#include "../ModelDataParserClass.h"

class TextureClass;
class ModelDataParserClass;
class ModelComponent : public Component
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
	ModelComponent();
	~ModelComponent();

	// �� �����Ӹ��� ȣ��
	virtual void UpdateComponent() override;

	bool Initialize(ID3D11Device* device, WCHAR* textureFilename, WCHAR* modelFileName = L"");
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

	ShaderManager::ShaderType GetShaderType();

	ID3D11ShaderResourceView* GetTexture();

	void SetPosition(float x, float y, float z);
	XMFLOAT3 GetPosition();

	void SetDiffuseColor(float x, float y, float z, float w);
	XMFLOAT4 GetDiffuseColor();

	void SetExtension(WCHAR* extension);

private:
	bool CheckAndModelLoader(WCHAR* modelFileName);
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

	XMFLOAT3 modelPosition;

	XMFLOAT4 modelDiffustColor;

	WCHAR* modelExtension = nullptr;
	WCHAR* parsedModelExtension = nullptr;

	ModelDataParserClass* modelParser = nullptr;
};

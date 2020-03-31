#pragma once

class ModelClass : public AlignedAllocationPolicy<16>
{
private:
	// 한 정점의 정보
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	ModelClass();
	~ModelClass();

	bool Initialize(ID3D11Device* device);
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

	int m_vertexCount = 0;
	int m_indexCount = 0;
};


#pragma once
#include <fstream>
#include <d3dcompiler.h>
using namespace std;

class LightShaderClass : public AlignedAllocationPolicy<16>
{
	// 쉐이더 입력 값과 같은 포맷
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBuffertype
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;				// D3D11_BIND_CONSTANT_BUFFER일 경우 버퍼가 항상 16의 배수여야 함 - 안그러면 CreateBuffer 실패함
	};

public:
	LightShaderClass();
	~LightShaderClass();

	bool Initialize(ID3D11Device* device, HWND hWnd);
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount,
		XMMATRIX worldMatrix,
		XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture,
		XMFLOAT3 lightDirection,
		XMFLOAT4 diffuseColor);

private:
	bool InitializeShader(ID3D11Device* device, HWND hWnd, WCHAR* vsFilename, WCHAR* psFilename);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext,
		XMMATRIX worldMatrix,
		XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture,
		XMFLOAT3 lightDirection,
		XMFLOAT4 diffuseColor);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D11Buffer* m_matrixBuffer = nullptr;
	ID3D11SamplerState* m_sampleState = nullptr;
	ID3D11Buffer* m_lightBuffer;
};


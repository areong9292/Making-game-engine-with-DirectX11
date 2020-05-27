#pragma once

#include "./DesignPattern/DesignPatternIncludes.h"
#include "./ShaderClass/ShaderClassIncludes.h"

class ColorShaderClass;
class TextureShaderClass;
class LightShaderClass;
class ShaderManager : Singleton<ShaderManager>
{
public:
	enum ShaderType
	{
		ColorShader,			// �ܼ� �÷��� ó���ϴ� ���̴�
		TextureShader,			// �ؽ��� ������ ���̴�
		LightShader,			// ���� ó���ϴ� ���̴�
	};

public:
	ShaderManager();
	~ShaderManager();

	bool Initialize(ID3D11Device* device, HWND hWnd);
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, ShaderType shaderType,
				XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
				ID3D11ShaderResourceView* texture = nullptr, XMFLOAT3 lightDirection = XMFLOAT3(), XMFLOAT4 diffuseColor = XMFLOAT4());

private:
	ColorShaderClass* m_colorShaderClass = nullptr;
	TextureShaderClass* m_textureShaderClass = nullptr;
	LightShaderClass* m_LightShaderClass = nullptr;
};

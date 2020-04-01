#pragma once

#include "./DesignPattern/DesignPatternIncludes.h"
#include "./ShaderClass/ShaderClassIncludes.h"

class ColorShaderClass;
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
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, ShaderType shaderType, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

private:
	ColorShaderClass* m_colorShaderClass = nullptr;
};

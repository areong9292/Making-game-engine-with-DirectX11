#pragma once

#include "./DesignPattern/DesignPatternIncludes.h"
#include "./ShaderClass/ShaderClassIncludes.h"

class ColorShaderClass;
class ShaderManager : Singleton<ShaderManager>
{
	enum ShaderType
	{
		ColorShader,			// 단순 컬러만 처리하는 쉐이더
		TextureShader,			// 텍스쳐 입히는 쉐이더
		LightShader,			// 빛을 처리하는 쉐이더
	};

public:
	ShaderManager();
	~ShaderManager();

	bool Initialize(ID3D11Device* device, HWND hWnd);
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
private:
	ColorShaderClass* m_colorShaderClass = nullptr;
};

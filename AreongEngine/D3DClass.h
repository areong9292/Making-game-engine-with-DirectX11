#pragma once

// 객체 모듈을 사용하기 위해 링크하는 라이브러리들 명시
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

// 링크한 라이브러리들, DirectX타입 정의와 같은 것들에 대한 헤더 추가
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class D3DClass
{
public:
	D3DClass();
	~D3DClass();

	// 필요한 정보를 AreongEngine으로 부터 제공받는다
	bool InitializeD3DClass(int screenWidth, int screenHeight, HWND hWnd);
	bool InitScene();
	void UpdateScene();
	void DrawScene();

private:

	IDXGISwapChain* swapChain;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;

	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;
	int colormodr = 1;
	int colormodg = 1;
	int colormodb = 1;
};
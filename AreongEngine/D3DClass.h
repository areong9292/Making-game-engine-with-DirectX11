#pragma once

// ��ü ����� ����ϱ� ���� ��ũ�ϴ� ���̺귯���� ���
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

// ��ũ�� ���̺귯����, DirectXŸ�� ���ǿ� ���� �͵鿡 ���� ��� �߰�
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

	// �ʿ��� ������ AreongEngine���� ���� �����޴´�
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
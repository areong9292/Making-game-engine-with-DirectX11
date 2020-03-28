#include "stdafx.h"
#include "D3DClass.h"


D3DClass::D3DClass()
{
	MessageBox(0, L"D3DClass", L"Error", MB_OK);
}

D3DClass::~D3DClass()
{
	// ���� COM ��ü ������
	if (SwapChain != nullptr)
		SwapChain->Release();

	if (d3d11Device != nullptr)
		d3d11Device->Release();

	if (d3d11DevCon != nullptr)
		d3d11DevCon->Release();

	MessageBox(0, L"~D3DClass", L"Error", MB_OK);
}

bool D3DClass::InitializeD3DClass(int screenWidth, int screenHeight, HWND hWnd)
{
	HRESULT result;

	// ������ ����

	// �� ���������� �ֻ��� �����ϰ� �׳� ����µ�
	// ����� �˻��ؼ� ������ �ֻ��� üũ�ؼ� ��������
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = screenWidth;
	bufferDesc.Height = screenHeight;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ����ü���� ����
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Device, DeviceContext, SwapChain ����
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

	if (FAILED(result))
	{
		MessageBox(0, L"D3D11CreateDeviceAndSwapChain - Failed",
			L"Error", MB_OK);
		return false;
	}

	// ����� ����
	ID3D11Texture2D* BackBuffer;
	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	if (FAILED(result))
	{
		MessageBox(0, L"SwapChain->GetBuffer - Failed",
			L"Error", MB_OK);
		return false;
	}

	// �� ���۷� ���� Ÿ�� �� ����
	// �� ���۸� ������ ������� �ؼ� �並 �����Ѵ�
	result = d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);

	if (FAILED(result))
	{
		MessageBox(0, L"d3d11Device->CreateRenderTargetView - Failed",
			L"Error", MB_OK);
		return false;
	}

	BackBuffer->Release();

	// ������ ��� ���� Ÿ���� �����Ѵ�
	d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, NULL);

	return true;
}

bool D3DClass::InitScene()
{
	return true;
}

void D3DClass::UpdateScene()
{
	// �� �����Ӹ��� �÷� �� �����Ѵ�
	red += colormodr * 0.00005f;
	green += colormodg * 0.00002f;
	blue += colormodb * 0.00001f;

	if (red >= 1.0f || red <= 0.0f)
		colormodr *= -1;
	if (green >= 1.0f || green <= 0.0f)
		colormodg *= -1;
	if (blue >= 1.0f || blue <= 0.0f)
		colormodb *= -1;
}

void D3DClass::DrawScene()
{
	float bgColor[4];

	// ���۸� � �������� ���� ������ ����
	bgColor[0] = red;
	bgColor[1] = green;
	bgColor[2] = blue;
	bgColor[3] = 1.0f;

	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);

	// ����۸� ������ ������
	SwapChain->Present(0, 0);
}
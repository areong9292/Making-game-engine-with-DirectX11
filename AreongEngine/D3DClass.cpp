#include "stdafx.h"
#include "D3DClass.h"

D3DClass::D3DClass()
{
	// ��� �����͵��� �ʱ�ȭ
	m_swapChain = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilBuffer = nullptr;
	m_depthStencilState = nullptr;
	m_depthStencilView = nullptr;
	m_rasterState = nullptr;

	m_depthDisabledStencilState = nullptr;

	m_alphaEnableBlendingState = nullptr;
	m_alphaDisableBlendingState = nullptr;
}

D3DClass::D3DClass(const D3DClass& other)
{
}


D3DClass::~D3DClass()
{
}

// ��ü Direct3D �ʱ�ȭ �����ϴ� �޼ҵ�
// screenWidth, screenHeight - SystemClass���� ������� �������� �ʺ�� ����, ������� ������ ũ���� ������ �ʱ�ȭ�ϰ� �̿��Ѵ�
// vsync - ���� ����ȭ �Ұ��� ����(����� �ֻ����� ���� ������ �Ҳ��� �ƴϸ� ������ �� ������ �ٽ� �׸��ų�)
// hwnd - SystemClass���� ���� ������ �ڵ�, �̸� �̿��Ͽ� �����쿡 ������
// screenDepth, screenNear - �����쿡 �׷��� 3Dȯ�濡���� ����
bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
	float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

	D3D11_BLEND_DESC blendStateDescription;

	// vsync(��������ȭ) ������ �����Ѵ�.
	m_vsync_enabled = vsync;

	// Direct3D �ʱ�ȭ �� �׷��� ī��/ ����� �ֻ���(���ΰ�ħ ����)�� �˾ƾ��Ѵ�
	// ��ǻ�͸��� �ٸ��� ������ �ش� ������ ��ȸ�ؾ��Ѵ�.

	// �ֻ��� ��� �۾��� ���� ������ DirectX�� ȭ���� ǥ���� ��
	// ���� �ø��� ������� �ʰ� blit�� ����ϰ� �Ǿ� ������ ���ϵǰ�
	// ����� ��¿� �Ž����� ���� �޼����� ��µȴ�

	// DXGI - DirectX Graphics Infrastructure�� ���ڷ� Direct3D�� �Բ� ���̴� api�̴�
	// �������� �׷��� ����� ó���ϴ� api�� �������� �� ���������� �ֻ��� ��� �۾��� ���δ�

	// DirectX �׷��� �������̽� ���丮�� �����
	// ����ü�� ����, �׷���ī��(����Ʈ������ �׷��� ī�嵵 ����) enum�� ���
	// �ش� ���������� ����ü���� Direct3D ��ġ ����鼭 ���� ����� ���� ���⼱ �ȸ���
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	/// �ƴ��ͷ� �׷���ī�忡 ���� ����
	// ������ ���丮�� �ٷ� �׷��� ī�� enum�� �����´� EnumAdapters
	// ���丮 ��ü�� ����Ͽ� ù��° �׷��� ī�� �������̽��� ���� �ƴ��͸� �����.
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// ���丮���� �׷��� ī�� �����ϰ� �׷��� ī�忡�� ����� �����´�
	// ���(�����)�� ���� ù��° �ƴ��͸� �����Ѵ�
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// �ش� ����Ͱ� ǥ���� �� �ִ� ȭ�� ���̿� ���̸� ����� �ϴ� ��
	// DXGI_FORMT_R8G8B8A8_UNORM ����� ��� ���÷��� ���˿� �´� ����� ������ ���Ѵ�
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// ������ ��� ����Ϳ� �׷���ī�� ������ ������ ����Ʈ�� �����Ѵ�.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// ���÷��� ��忡 ���� ����Ʈ ������ ä���ִ´�
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// ���� ��� ���÷��� ��忡 ���� ȭ�� �ʺ�/ ���̿� �´� ���÷��� ��带 ã�´�
	// ������ ���� ã���� ������� ���ΰ�ħ ������ �и�� ���� ���� �����Ѵ�
	// �� ������ ����/�и� ���� ��ȸ�� �� ���� �߿� DirectX�� �� ���� �˷��ָ� ������ �ֻ����� ����ϰԵȴ�.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// �����(�׷���ī��)�� description(����)�� �����´�.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	/// �׷��� ī���� ��� ������ �޸� ũ��� �̸��� �����´�
	// ���� �׷��� ī���� �޸� �뷮�� �ް�����Ʈ ������ �����Ѵ�.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// �׷���ī���� �̸��� char�� ���ڿ� �迭�� �ٲ� �� �����Ѵ�.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	/// �׷���ī�� ������ ������� ����ߴ� ����ü��� �������̽� ����
	// ���÷��� ��� ����Ʈ�� �Ҵ��� �����Ѵ�
	delete[] displayModeList;
	displayModeList = nullptr;

	// ��� �ƴ���(�����)�� �Ҵ� ����
	adapterOutput->Release();
	adapterOutput = nullptr;

	// �ƴ���(�׷��� ī��)�� �Ҵ� ����
	adapter->Release();
	adapter = nullptr;

	// ���丮(�׷��� ī�忡 �����ϱ� ������ �����) ��ü�� �Ҵ� ����
	factory->Release();
	factory = nullptr;


	/// ������� �ý��ۿ��� �ֻ����� ����
	/// �ý��� �ֻ��� ������ �� DXGI�� ����ϴ� ���� �� �� �ִ�

	/// ����ü�� description ����ü ����
	// �� ���������� DXGI�� ����ü�� ������ ���ϰ� Direct3D�� �����Ѵ�
	// ����ۿ� ����� �׸��� ����Ʈ���ۿ� �ٲ�ġ��(swap) �Ͽ� ������ ȭ�鿡 ���̰� �ȴ�
	// �׷��� ����ü���̶�� �Ҹ�

	// ����ü�� description �ʱ�ȭ
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// �ϳ��� ����۸� ���
	swapChainDesc.BufferCount = 1;

	// ������� �ʺ�� ���̸� ���� - SystemClass���� ���� ������ ũ�⸸ŭ�� ���۸� ���
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// ����۷� �Ϲ����� 32bit�� �����̽�(ȭ��, ǥ��)�� ����
	// ����� ��� ������ �� ����� DXGI_FORMAT_R8G8B8A8_UNORM ���˰� ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;


	/// ����� ���� �� �ֻ��� ����
	/// �ֻ��� - 1�ʴ� �� ���� ����۸� ����Ʈ ���ۿ� �ٲ�ġ�� �� ���̳�

	// ������� �ֻ��� ����
	if (m_vsync_enabled)
	{
		// ȭ�� �ʺ�� �´� ����� ���� ����� �ֻ����� �����ȴ�
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		// 1�ʿ� �ִ��� ������ ȭ���� �׸��� - ȭ�� �Ϻο� ������ ���� �� �ִ�
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// ������� �뵵 ���� - ���� Ÿ������ ����� �Ŵ�
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// �������� �̷���� �������� �ڵ��� ����
	swapChainDesc.OutputWindow = hwnd;

	// ��Ƽ���ø� off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// ������ ���� ������ ��� �Ǵ� Ǯ��ũ�� ���� ����
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// ��ĵ������ ���İ� ��ĵ���̴��� �������� ����(unspecified)���� ����
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ��µ� ������ ������� ������ �������� ����
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// �߰� �ɼ� �÷��׸� ������� ����
	swapChainDesc.Flags = 0;


	// ���� ����(� ������ DirectX�� ����� ���ΰ�)�� DirectX11�� ����
	featureLevel = D3D_FEATURE_LEVEL_11_0;


	/// ����ü�� description ����ü�� ���ķ����� ä������
	/// ����ü��, Direct3D ��ġ, Direct3D ��ġ ���ؽ�Ʈ�� ���� �� �ִ�.

	// ����ü��, Direct3D ����̽�, Direct3D ����̽� ���ؽ�Ʈ ����
	// D3D_DRIVER_TYPE_HARDWARE�� D3D_DRIVER_TYPE_REFERENCE�� �ٲٸ�
	// �׷��� ī�尡 DirectX11�� �������� �ʾƵ� CPU���� ������ ó���ϰ� �Ǿ� 11�� ����� ����� �� �ִ�
	result = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,		// ����̹� Ÿ�� - DirectX 11�� �����ϴ� �׷���ī��
		NULL,
		0,								// ����̹� �÷���
		&featureLevel,					// ��ó���� �迭
		1,								// ��ó���� ī��Ʈ
		D3D11_SDK_VERSION,				// SDK����
		&swapChainDesc,					// ����ü�� ���� ����ü
		&m_swapChain,					// ������ ����ü��
		&m_device,						// ������ ����̽� ��ü
		NULL,							// ������ ��ó���� ����
		&m_deviceContext);				// ������ ����̽� ���ؽ�Ʈ ��ü

	if (FAILED(result))
	{
		return false;
	}

	// ���� ù��°(�⺻) �׷���ī��� ���� �׷���, �ι�°�� ���� �׷������� ���� �� �ִ�.
	// �̷� ��츦 ���� ���� �⺻ �׷���ī�常 ����ϸ� �ȵǰ� ����� ��� ���� ī�带 �����Ͽ�
	// �������� ���� �� �´� �׷��� ī��� ��ġ�� ������ �� �ְ� �ؾ��Ѵ�.
	// �� ��� ã�ƺ� ��



	/// ����ü�� ��ġ�� �����Ǿ����� ����� �����͸� �޾ƿ� ����ü�ο� �����ؾ��Ѵ�.
	// _uuidof ������ ǥ���Ŀ� ������ Ŭ�������̳� �������̽��� ���� GUID���� �������ش�.
	// ���⼱ ID3D11Texture2D�� GUID���� ���� - ����۴� �⺻������ 2D �ؽ��ķ� ������� �ִ�

	// ������ ����ü������ ù��° ������� �����͸� �����´�.
	result = m_swapChain->GetBuffer(
		0,										// ������� ��ȣ(�ε���)
		__uuidof(ID3D11Texture2D),				// ����ۿ� �����ϴ� �������̽�
		(LPVOID*)&backBufferPtr);				// ����۸� �޾ƿ� ����

	if (FAILED(result))
	{
		return false;
	}

	// �ؽ��Ĵ� �������������κ��� �並 ���� ������ ����
	// ����Ÿ�ٵ� �ؽ����� �������� ���� �並 �̿��Ͽ� �����ؾ��Ѵ�

	// ������� �����ͷ� ����Ÿ�� �並 ����
	result = m_device->CreateRenderTargetView(
		backBufferPtr,			// �信�� ������ �ϴ� ���ҽ�(�����)
		NULL,					// ����Ÿ�� ���� ����
		&m_renderTargetView);	// ����Ÿ�� �並 �޾ƿ� ����

	if (FAILED(result))
	{
		return false;
	}

	// ����Ÿ�� �䰡 ������ ���Ŀ��� ���� ������� �����Ϳ� �������� �ʰ�
	// ����Ÿ�� �並 ����ؼ� �����Ѵ�.
	// ����� �����͸� ���̻� ������� �����Ƿ� �Ҵ� ����
	backBufferPtr->Release();
	backBufferPtr = nullptr;


	/// ���� ������ description ����ü ���� - �̸� �̿��� ���� ���۸� ������
	/// 3D �������� ��������� �ùٸ��� �׷�����.
	/// ���ÿ� ���ٽ� ���۵� ���̹��ۿ� �����Ѵ�.
	/// ���ٽǹ��۴� ��Ǻ�, ������Ʈ�� �׸��� ���� ȿ���� ���� ����Ѵ�.
	// ���� ������ description�� �ʱ�ȭ
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// ���� ������ description�� ����
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// description�� ����� ���̹��� �ؽ��ĸ� �����Ѵ�.
	// ���� ���� 2D �ؽ����̴�.
	// ��������� ���ĵǰ� ������ȭ�� ���� 2D��ǥ�� �ȼ����� �Ǳ� ����
	// ���� �� 2D���۰� ȭ�鿡 �׷�����.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// ���ٽ� ������ description�� �ʱ�ȭ
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// ���ٽ� ������ description�� ����
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// ���ٽ� ���� �۵�
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// ���ٽ� �ĸ� �۵�
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	// ����-���ٽ� ���� ������ ����
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// ����-���ٽ� ���� ������ ����(��ġ ���ؽ�Ʈ ���)
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);


	/// ����-���ٽ� ������ �信 ���� description
	/// �� �۾��� �ؾ� Direct3D�� ���̹��۸� ����-���ٽ� �ؽ��ķ� �ν���
	// ����-���ٽ� ���� description�� �ʱ�ȭ
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// ����-���ٽ� ���� description�� �ۼ�
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// ����-���ٽ� �並 ����
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	/// ���� OBSetRenderTarget �Լ��� ȣ���� �� �ִ�.
	/// �� �Լ��� ����Ÿ�ٺ�� ����-���ٽ� �並 ��� ������ ���������ο� ���ε���Ų��.
	/// ������������ �̿��� �������� ����� �� �츮�� ������� ����ۿ� ����� �׷�����
	/// �� ����ۿ� �׷��� ���� ����Ʈ ���ۿ� �ٲ�ġ�� �Ͽ� ����Ϳ� ����Ѵ�.

	// ����Ÿ�� ��� ����-���ٽ� ���۸� ���� ��� ���������ο� ���ε�
	m_deviceContext->OMSetRenderTargets(
		1,						// ����Ÿ���� �� - �ִ� 8��
		&m_renderTargetView,	// ����Ÿ�� ���� �迭
		m_depthStencilView);	// ����-���ٽ� ���� ������


/// ������ ��� �ȼ��� �׷��������� ���� ��� �� �� �ְ��Ѵ�.
/// ȭ���� ���̾������Ӹ��� �׸��ų� ������ �յ޸��� ��� �׸����� �� ���� �ִ�.
// � ������ ��� �׸� ������ �����ϴ� ������ȭ�� description�� ����
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// �ۼ��� description���κ��� ������ȭ�� ���¸� ����
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// ������ȭ�� ���¸� ����
	m_deviceContext->RSSetState(m_rasterState);

	/// ����Ʈ - ����Ÿ���� ������ ������ ���� ����
	/// ����Ʈ�� �� ����Ÿ�ٺ��� �����ؾ��Ѵ�.
	/// ����Ʈ�� �־�� ����Ÿ�� ������ Ŭ������ ������ �� �ִ�.
	// �������� ���� ����Ʈ�� ����
	viewport.Width = (float)screenWidth;		// ����Ʈ ������ ����
	viewport.Height = (float)screenHeight;		// ����Ʈ ������ ����
	viewport.MinDepth = 0.0f;					// ����Ʈ ������ ���̰��� �ּҰ�
	viewport.MaxDepth = 1.0f;					// ����Ʈ ������ ���̰��� �ִ밪
	viewport.TopLeftX = 0.0f;					// ����Ʈ ������ �»� X��ǥ
	viewport.TopLeftY = 0.0f;					// ����Ʈ ������ �»� Y��ǥ

	// ����Ʈ�� ����
	// RS - �����Ͷ�����, ���������ο��� �����Ͷ����� ���������� ����
	m_deviceContext->RSSetViewports(
		1,				// ������ ����Ʈ ����
		&viewport);		// ����Ʈ ������ ����Ʈ ����ü �迭


	/// ���� ��� - 3Dȭ���� �ռ� ���� 2D����Ʈ �������� ��ȯ
	// ���� ����� ����
	fieldOfView = (float)XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// 3D �������� ���� ���� ����� ����
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	/// ���� ��� - ������Ʈ���� ��ǥ�� 3D������ ��ǥ�� ��ȯ, 3���� ���������� ȸ��/�̵�/ũ�� ��ȯ���� ���
	// ���� ����� ���� ��ķ� �ʱ�ȭ
	m_worldMatrix = XMMatrixIdentity();

	/// �� ��� - ���� ��鿡�� �츮�� ��� ��ġ���� ��� ������ ���� �ִ°��� ���
	/// ī�޶� ���� ��ķ� ���� ī�޶� Ŭ�������� ����

	/// ���� ���� ��� - UI�� ���� 2D�� ��ҵ��� �׸��� ���� ���
	// 2D �������� ���� ���� ���� ����� ����
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	// 2d�� ����-���ٽ� desc�� �ۼ��Ѵ�
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// 2d�� �׸������� DepthEnable�� false���Ѵ�
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// ����-���ٽ��� �����Ѵ�
	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// ���� ���� desc �ʱ�ȭ
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// ���� �� ������ ����
	// BlendEnable = TRUE, DestBlend = D3D11_BLEND_INV_SRC_ALPHA�� ����
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// ���İ� ���� ���� ���¸� �����Ѵ�
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	// ���İ� ���� ���� BlendEnable�� FALSE�� �����ϰ�
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// ���� ���� �����Ѵ�
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

// DirectX11 �ʱ�ȭ�� ����� �����͵��� �����ϰ� ����
void D3DClass::Shutdown()
{
	// �����ϱ� �� ������ ���� �ٲ��� ������ ����ü���� �Ҵ������� �� ���ܰ� �߻�
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_alphaEnableBlendingState != nullptr)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = nullptr;
	}

	if (m_alphaDisableBlendingState != nullptr)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = nullptr;
	}

	if (m_depthDisabledStencilState != nullptr)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = nullptr;
	}

	if (m_rasterState != nullptr)
	{
		m_rasterState->Release();
		m_rasterState = nullptr;
	}

	if (m_depthStencilView != nullptr)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}

	if (m_depthStencilState != nullptr)
	{
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}

	if (m_depthStencilBuffer != nullptr)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
	}

	if (m_renderTargetView != nullptr)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_deviceContext != nullptr)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}
	if (m_device != nullptr)
	{
		m_device->Release();
		m_device = nullptr;
	}
	if (m_swapChain != nullptr)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}

	return;
}

// �� �������� ���۸��� 3Dȭ���� �׸��� ������ �� ȣ��
// ���۸� �� ������ �ʱ�ȭ �� �������� �̷�������� �غ�
void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	// ���۸� � �������� ���� ������ ����
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// ������� ������ ����
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// ���� ���۸� ����
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

// �� �������� �������� ���� ü�ο��� ����ۿ� �׸� 3Dȭ���� ǥ��
void D3DClass::EndScene()
{
	// �������� �Ϸ�Ǿ����Ƿ� ������� ������ ȭ�鿡 ���
	if (m_vsync_enabled)
	{
		// ���ΰ�ħ ������ ����
		m_swapChain->Present(1, 0);
	}
	else
	{
		// ������ ������ ǥ��
		m_swapChain->Present(0, 0);
	}
	return;
}

// Direct3D ����̽� ������ ����
ID3D11Device * D3DClass::GetDevice()
{
	return m_device;
}

// Direct3D ����̽� ���ؽ�Ʈ ������ ����
ID3D11DeviceContext * D3DClass::GetDeviceContext()
{
	return m_deviceContext;
}

/// ��κ��� ���̴����� ����, ����, �������� ����� �ʿ��ϴ�
// ������� ��ȯ
void D3DClass::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}

// ������� ��ȯ
void D3DClass::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}

// ����������� ��ȯ
void D3DClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

// �׷��� ī���� �̸��� ��� ������ �޸��� ���� ����
void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

// Z���۸� Ų��
void D3DClass::TurnZBufferOn()
{
	// ų���� ������ ���� ���ٽ� ���¸� ����Ѵ�
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	return;
}

// Z���۸� ����
void D3DClass::TurnZBufferOff()
{
	// ������ DepthEnable�� false�� �� ���� ���ٽ� ���¸� ����Ѵ�
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	return;
}

// ���� ���� Ų��
void D3DClass::TurnOnAlphaBlending()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}

// ���� ���� ����
void D3DClass::TurnOffAlphaBlending()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

ID3D11RenderTargetView* D3DClass::GetRenderTargetView()
{
	return m_renderTargetView;
}

ID3D11DepthStencilView * D3DClass::GetDepthStencilView()
{
	return m_depthStencilView;
}

void D3DClass::SetBackBufferRenderTarget()
{
	// ��� ������ ���������ο� ���� Ÿ�� ��� ���� ���ٽ� ���۸� ���ε�
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	return;
}

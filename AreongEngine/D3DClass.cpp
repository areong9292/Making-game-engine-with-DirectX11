#include "stdafx.h"
#include "D3DClass.h"

D3DClass::D3DClass()
{
	// 멤버 포인터들을 초기화
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

// 전체 Direct3D 초기화 설정하는 메소드
// screenWidth, screenHeight - SystemClass에서 만들었던 윈도우의 너비와 높이, 윈도우와 동일한 크기의 영역을 초기화하고 이용한다
// vsync - 수직 동기화 할건지 여부(모니터 주사율에 맞춰 렌더링 할꺼냐 아니면 가능한 한 빠르게 다시 그릴거냐)
// hwnd - SystemClass에서 만든 윈도우 핸들, 이를 이용하여 윈도우에 접근함
// screenDepth, screenNear - 윈도우에 그려질 3D환경에서의 깊이
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

	// vsync(수직동기화) 설정을 저장한다.
	m_vsync_enabled = vsync;

	// Direct3D 초기화 전 그래픽 카드/ 모니터 주사율(새로고침 비율)을 알아야한다
	// 컴퓨터마다 다르기 때문에 해당 정보를 조회해야한다.

	// 주사율 계산 작업을 하지 않으면 DirectX가 화면을 표시할 때
	// 버퍼 플립을 사용하지 않고 blit를 사용하게 되어 성능이 저하되고
	// 디버그 출력에 거슬리는 에러 메세지가 출력된다

	// DXGI - DirectX Graphics Infrastructure의 약자로 Direct3D와 함께 쓰이는 api이다
	// 공통적인 그래픽 기능을 처리하는 api의 집합으로 이 예제에서는 주사율 계산 작업에 쓰인다

	// DirectX 그래픽 인터페이스 팩토리를 만든다
	// 스왑체인 생성, 그래픽카드(소프트웨어적 그래픽 카드도 가능) enum에 사용
	// 해당 예제에서는 스왑체인은 Direct3D 장치 만들면서 같이 만들기 위해 여기선 안만듬
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	/// 아답터로 그래픽카드에 접근 가능
	// 생성한 팩토리로 바로 그래픽 카드 enum을 가져온다 EnumAdapters
	// 팩토리 객체를 사용하여 첫번째 그래픽 카드 인터페이스에 대한 아답터를 만든다.
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// 팩토리에서 그래픽 카드 접근하고 그래픽 카드에서 모니터 가져온다
	// 출력(모니터)에 대한 첫번째 아답터를 나열한다
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// 해당 모니터가 표시할 수 있는 화면 넓이와 높이를 모드라고 하는 듯
	// DXGI_FORMT_R8G8B8A8_UNORM 모니터 출력 디스플레이 포맷에 맞는 모드의 개수를 구한다
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// 가능한 모든 모니터와 그래픽카드 조합을 저장할 리스트를 생성한다.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// 디스플레이 모드에 대한 리스트 구조를 채워넣는다
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// 이제 모든 디스플레이 모드에 대해 화면 너비/ 높이에 맞는 디스플레이 모드를 찾는다
	// 적합한 것을 찾으면 모니터의 새로고침 비율의 분모와 분자 값을 저장한다
	// 이 비율의 분자/분모 값을 조회한 뒤 설정 중에 DirectX에 그 값을 알려주면 적절한 주사율을 계산하게된다.
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

	// 어댑터(그래픽카드)의 description(설명)을 가져온다.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	/// 그래픽 카드의 사용 가능한 메모리 크기와 이름을 가져온다
	// 현재 그래픽 카드의 메모리 용량을 메가바이트 단위로 저장한다.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 그래픽카드의 이름을 char형 문자열 배열로 바꾼 뒤 저장한다.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	/// 그래픽카드 정보를 얻기위해 사용했던 구조체들과 인터페이스 해제
	// 디스플레이 모드 리스트의 할당을 해제한다
	delete[] displayModeList;
	displayModeList = nullptr;

	// 출력 아답터(모니터)를 할당 해제
	adapterOutput->Release();
	adapterOutput = nullptr;

	// 아답터(그래픽 카드)를 할당 해제
	adapter->Release();
	adapter = nullptr;

	// 팩토리(그래픽 카드에 접근하기 용으로 사용한) 객체를 할당 해제
	factory->Release();
	factory = nullptr;


	/// 여기까지 시스템에서 주사율을 얻어옴
	/// 시스템 주사율 가져올 때 DXGI를 사용하는 것을 알 수 있다

	/// 스왑체인 description 구조체 설정
	// 이 예제에서는 DXGI로 스왑체인 생성을 안하고 Direct3D로 생성한다
	// 백버퍼에 장면을 그리고 프론트버퍼와 바꿔치기(swap) 하여 유저의 화면에 보이게 된다
	// 그래서 스왑체인이라고 불림

	// 스왑체인 description 초기화
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// 하나의 백버퍼만 사용
	swapChainDesc.BufferCount = 1;

	// 백버퍼의 너비와 높이를 설정 - SystemClass에서 만든 윈도우 크기만큼의 버퍼를 사용
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// 백버퍼로 일반적인 32bit의 서페이스(화면, 표면)을 지정
	// 모니터 모드 가져올 때 사용한 DXGI_FORMAT_R8G8B8A8_UNORM 포맷과 같다
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;


	/// 백버퍼 설정 후 주사율 셋팅
	/// 주사율 - 1초당 몇 장의 백버퍼를 프론트 버퍼와 바꿔치기 할 것이냐

	// 백버퍼의 주사율 설정
	if (m_vsync_enabled)
	{
		// 화면 너비와 맞는 모니터 모드로 계산한 주사율로 고정된다
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		// 1초에 최대한 빠르게 화면을 그린다 - 화면 일부에 결점을 남길 수 있다
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// 백버퍼의 용도 설정 - 렌더 타겟으로 사용할 거다
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 렌더링이 이루어질 윈도우의 핸들을 설정
	swapChainDesc.OutputWindow = hwnd;

	// 멀티샘플링 off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// 백퍼퍼 역시 윈도우 모드 또는 풀스크린 모드로 설정
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// 스캔라인의 정렬과 스캔라이닝을 지정되지 않음(unspecified)으로 설정
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 출력된 이후의 백버퍼의 내용을 버리도록 설정
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 추가 옵션 플래그를 사용하지 않음
	swapChainDesc.Flags = 0;


	// 피쳐 레벨(어떤 버전의 DirectX를 사용할 것인가)을 DirectX11로 설정
	featureLevel = D3D_FEATURE_LEVEL_11_0;


	/// 스왑체인 description 구조체와 피쳐레벨이 채워지면
	/// 스왑체인, Direct3D 장치, Direct3D 장치 컨텍스트를 만들 수 있다.

	// 스왑체인, Direct3D 디바이스, Direct3D 디바이스 컨텍스트 생성
	// D3D_DRIVER_TYPE_HARDWARE를 D3D_DRIVER_TYPE_REFERENCE로 바꾸면
	// 그래픽 카드가 DirectX11을 지원하지 않아도 CPU에서 렌더링 처리하게 되어 11의 기능을 사용할 수 있다
	result = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,		// 드라이버 타입 - DirectX 11을 지원하는 그래픽카드
		NULL,
		0,								// 드라이버 플래그
		&featureLevel,					// 피처레벨 배열
		1,								// 피처레벨 카운트
		D3D11_SDK_VERSION,				// SDK버전
		&swapChainDesc,					// 스왑체인 생성 구조체
		&m_swapChain,					// 생성된 스왑체인
		&m_device,						// 생성된 디바이스 객체
		NULL,							// 생성된 피처레벨 정보
		&m_deviceContext);				// 생성된 디바이스 컨텍스트 객체

	if (FAILED(result))
	{
		return false;
	}

	// 가끔 첫번째(기본) 그래픽카드는 내장 그래픽, 두번째가 외장 그래픽으로 잡힐 수 있다.
	// 이런 경우를 위해 절대 기본 그래픽카드만 사용하면 안되고 기기의 모든 비디오 카드를 나열하여
	// 유저에게 가장 잘 맞는 그래픽 카드로 장치를 생성할 수 있게 해야한다.
	// 요 방법 찾아볼 것



	/// 스왑체인 장치가 생성되었으니 백버퍼 포인터를 받아와 스왑체인에 연결해야한다.
	// _uuidof 예약어는 표현식에 지정된 클래스명이나 인터페이스명에 대한 GUID값을 리턴해준다.
	// 여기선 ID3D11Texture2D의 GUID값을 리턴 - 백버퍼는 기본적으로 2D 텍스쳐로 만들어져 있다

	// 생성한 스왑체인으로 첫번째 백버퍼의 포인터를 가져온다.
	result = m_swapChain->GetBuffer(
		0,										// 백버퍼의 번호(인덱스)
		__uuidof(ID3D11Texture2D),				// 백버퍼에 접근하는 인터페이스
		(LPVOID*)&backBufferPtr);				// 백버퍼를 받아올 변수

	if (FAILED(result))
	{
		return false;
	}

	// 텍스쳐는 파이프라인으로부터 뷰를 통해 접근이 가능
	// 렌더타겟도 텍스쳐의 일종으로 역시 뷰를 이용하여 접근해야한다

	// 백버퍼의 포인터로 렌더타겟 뷰를 생성
	result = m_device->CreateRenderTargetView(
		backBufferPtr,			// 뷰에서 엑세스 하는 리소스(백버퍼)
		NULL,					// 렌더타겟 뷰의 정의
		&m_renderTargetView);	// 렌더타겟 뷰를 받아올 변수

	if (FAILED(result))
	{
		return false;
	}

	// 렌더타겟 뷰가 생성된 이후에는 직접 백버퍼의 포인터에 접근하지 않고
	// 렌더타겟 뷰를 사용해서 접근한다.
	// 백버퍼 포인터를 더이상 사용하지 않으므로 할당 해제
	backBufferPtr->Release();
	backBufferPtr = nullptr;


	/// 깊이 버퍼의 description 구조체 설정 - 이를 이용해 깊이 버퍼를 만들어야
	/// 3D 공간에서 폴리곤들이 올바르게 그려진다.
	/// 동시에 스텐실 버퍼도 깊이버퍼에 연결한다.
	/// 스텐실버퍼는 모션블러, 볼류메트릭 그림자 등의 효과를 낼때 사용한다.
	// 깊이 버퍼의 description을 초기화
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// 깊이 버퍼의 description을 설정
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

	// description을 사용해 깊이버퍼 텍스쳐를 생성한다.
	// 버퍼 역시 2D 텍스쳐이다.
	// 폴리곤들이 정렬되고 래스터화된 이후 2D좌표의 픽셀들이 되기 때문
	// 이제 이 2D버퍼가 화면에 그려진다.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 스텐실 상태의 description을 초기화
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// 스텐실 상태의 description을 설정
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// 스텐실 전면 작동
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 스텐실 후면 작동
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	// 깊이-스텐실 상태 변수를 생성
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// 깊이-스텐실 상태 변수를 설정(장치 컨텍스트 사용)
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);


	/// 깊이-스텐실 버퍼의 뷰에 대한 description
	/// 이 작업을 해야 Direct3D가 깊이버퍼를 깊이-스텐실 텍스쳐로 인식함
	// 깊이-스텐실 뷰의 description을 초기화
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// 깊이-스텐실 뷰의 description을 작성
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이-스텐실 뷰를 생성
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	/// 이제 OBSetRenderTarget 함수를 호출할 수 있다.
	/// 이 함수는 렌더타겟뷰와 깊이-스텐실 뷰를 출력 렌더링 파이프라인에 바인딩시킨다.
	/// 파이프라인을 이용한 렌더링이 수행될 때 우리가 만들었던 백버퍼에 장면이 그려지고
	/// 그 백버퍼에 그려진 것을 프론트 버퍼와 바꿔치기 하여 모니터에 출력한다.

	// 렌더타겟 뷰와 깊이-스텐실 버퍼를 각각 출력 파이프라인에 바인딩
	m_deviceContext->OMSetRenderTargets(
		1,						// 렌더타겟의 수 - 최대 8개
		&m_renderTargetView,	// 렌더타겟 뷰의 배열
		m_depthStencilView);	// 깊이-스텐실 뷰의 포인터


/// 도형이 어떻게 픽셀로 그려지는지에 대한 제어를 할 수 있게한다.
/// 화면을 와이어프레임모드로 그리거나 도형의 앞뒷면을 모두 그리도록 할 수도 있다.
// 어떤 도형을 어떻게 그릴 것인지 결정하는 래스터화기 description을 설정
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

	// 작성한 description으로부터 래스터화기 상태를 생성
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// 래스터화기 상태를 설정
	m_deviceContext->RSSetState(m_rasterState);

	/// 뷰포트 - 렌더타겟의 렌더링 영역에 관한 설정
	/// 뷰포트는 각 렌더타겟별로 설정해야한다.
	/// 뷰포트가 있어야 렌더타겟 공간에 클리핑을 수행할 수 있다.
	// 렌더링을 위한 뷰포트를 설정
	viewport.Width = (float)screenWidth;		// 뷰포트 영역의 넓이
	viewport.Height = (float)screenHeight;		// 뷰포트 영역의 높이
	viewport.MinDepth = 0.0f;					// 뷰포트 영역의 깊이값의 최소값
	viewport.MaxDepth = 1.0f;					// 뷰포트 영역의 깊이값의 최대값
	viewport.TopLeftX = 0.0f;					// 뷰포트 영역의 좌상 X좌표
	viewport.TopLeftY = 0.0f;					// 뷰포트 영역의 좌상 Y좌표

	// 뷰포트를 생성
	// RS - 래스터라이저, 파이프라인에서 레스터라이저 스테이지의 설정
	m_deviceContext->RSSetViewports(
		1,				// 설정할 뷰포트 갯수
		&viewport);		// 뷰포트 설정할 뷰포트 구조체 배열


	/// 투영 행렬 - 3D화면을 앞서 만든 2D뷰포트 공간으로 변환
	// 투영 행렬을 설정
	fieldOfView = (float)XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// 3D 렌더링을 위한 투영 행렬을 생성
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	/// 월드 행렬 - 오브젝트들의 좌표를 3D세계의 좌표로 변환, 3차원 공간에서의 회전/이동/크기 변환에도 사용
	// 월드 행렬을 단위 행렬로 초기화
	m_worldMatrix = XMMatrixIdentity();

	/// 뷰 행렬 - 현재 장면에서 우리가 어느 위치에서 어느 방향을 보고 있는가를 계산
	/// 카메라에 대한 행렬로 이후 카메라 클래스에서 생성

	/// 직교 투영 행렬 - UI와 같은 2D의 요소들을 그리기 위해 사용
	// 2D 렌더링에 사용될 직교 투영 행렬을 생성
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	// 2d용 깊이-스텐실 desc를 작성한다
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// 2d를 그리기위해 DepthEnable을 false로한다
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

	// 깊이-스텐실을 생성한다
	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// 블렌드 상태 desc 초기화
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// 알파 값 적용을 위해
	// BlendEnable = TRUE, DestBlend = D3D11_BLEND_INV_SRC_ALPHA로 지정
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// 알파값 켜진 블렌딩 상태를 생성한다
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	// 알파값 끄기 위해 BlendEnable만 FALSE로 지정하고
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// 블렌딩 상태 생성한다
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

// DirectX11 초기화에 사용한 포인터들을 해제하고 정리
void D3DClass::Shutdown()
{
	// 종료하기 전 윈도우 모드로 바꾸지 않으면 스왑체인을 할당해제할 때 예외가 발생
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

// 매 프레임의 시작마다 3D화면을 그리기 시작할 때 호출
// 버퍼를 빈 값으로 초기화 후 렌더링이 이루어지도록 준비
void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	// 버퍼를 어떤 색상으로 지울 것인지 설정
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// 백버퍼의 내용을 지움
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// 깊이 버퍼를 지움
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

// 매 프레임의 마지막에 스왑 체인에게 백버퍼에 그린 3D화면을 표시
void D3DClass::EndScene()
{
	// 렌더링이 완료되었으므로 백버퍼의 내용을 화면에 출력
	if (m_vsync_enabled)
	{
		// 새로고침 비율을 고정
		m_swapChain->Present(1, 0);
	}
	else
	{
		// 가능한 빠르게 표시
		m_swapChain->Present(0, 0);
	}
	return;
}

// Direct3D 디바이스 포인터 리턴
ID3D11Device * D3DClass::GetDevice()
{
	return m_device;
}

// Direct3D 디바이스 컨텍스트 포인터 리턴
ID3D11DeviceContext * D3DClass::GetDeviceContext()
{
	return m_deviceContext;
}

/// 대부분의 쉐이더에선 투영, 월드, 직교투영 행렬이 필요하다
// 투영행렬 반환
void D3DClass::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}

// 월드행렬 반환
void D3DClass::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}

// 직교투영행렬 반환
void D3DClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

// 그래픽 카드의 이름과 사용 가능한 메모리의 양을 리턴
void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

// Z버퍼를 킨다
void D3DClass::TurnZBufferOn()
{
	// 킬때는 원래의 깊이 스텐실 상태를 사용한다
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	return;
}

// Z버퍼를 끈다
void D3DClass::TurnZBufferOff()
{
	// 끌때는 DepthEnable을 false로 한 깊이 스텐실 상태를 사용한다
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	return;
}

// 알파 블렌드 킨다
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

// 알파 블렌드 끈다
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
	// 출력 렌더링 파이프라인에 렌더 타겟 뷰와 깊이 스텐실 버퍼를 바인딩
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	return;
}

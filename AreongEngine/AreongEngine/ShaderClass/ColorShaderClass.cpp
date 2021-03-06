#include "../stdafx.h"
#include "ColorShaderClass.h"

ColorShaderClass::ColorShaderClass()
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_layout = nullptr;
	m_matrixBuffer = nullptr;
}

// 버텍스 쉐이더, 픽셀 쉐이더 관련들을 반환한다
ColorShaderClass::~ColorShaderClass()
{	
	// 상수 버퍼를 해제한다
	if (m_matrixBuffer != nullptr)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}

	// 레이아웃을 해제한다
	if (m_layout != nullptr)
	{
		m_layout->Release();
		m_layout = nullptr;
	}

	// 픽셀 셰이더를 해제한다
	if (m_pixelShader != nullptr)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	// 정점 셰이더를 해제한다
	if (m_vertexShader != nullptr)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}

	MessageBox(0, L"~ColorShaderClass", L"Error", MB_OK);
}

bool ColorShaderClass::Initialize(ID3D11Device* device, HWND hWnd)
{
	bool result;

	// 쉐이더를 초기화하는 메소드
	// HLSL 쉐이더 파일의 이름을 넘겨준다
	result = InitializeShader(device, hWnd, L"./HLSL/Color.vs", L"./HLSL/Color.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

bool ColorShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;

	// 렌더링에 사용할 쉐이더의 인자를 입력한다
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// 쉐이더로 준비된 버퍼를 그린다
	RenderShader(deviceContext, indexCount);

	return true;
}

// 쉐이더 파일을 불러오고 DirectX와 GPU에서 사용 가능하도록 한다
bool ColorShaderClass::InitializeShader(ID3D11Device* device, HWND hWnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;

	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	// 이 쉐이더에서 입력 요소를 2개(position, color)를 사용한다
	// 입력 요소 벡터에 대한 2개의 입력 레이아웃 생성이 필요하다
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// 이 함수에서 사용하는 포인터들을 null로 설정
	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	// 쉐이더 프로그램을 버퍼로 컴파일한다
	// 정점 셰이더를 컴파일한다
	result = D3DCompileFromFile(
		vsFilename,						// 쉐이더 파일 이름
		NULL,
		NULL,
		"ColorVertexShader",			// 쉐이더 이름
		"vs_5_0",						// 쉐이더 버전
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&vertexShaderBuffer,			// 쉐이더가 컴파일 될 버퍼
		&errorMessage					// 에러 문자열
	);

	if (FAILED(result))
	{
		// 셰이더가 컴파일에 실패하면 에러 메세지를 기록
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hWnd, vsFilename);
		}
		// 에러 메세지가 없다면 셰이더 파일을 찾지 못한 것
		else
		{
			MessageBox(hWnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// 픽셀 셰이더를 컴파일한다
	result = D3DCompileFromFile(
		psFilename,						// 쉐이더 파일 이름
		NULL,
		NULL,
		"ColorPixelShader",				// 쉐이더 이름
		"ps_5_0",						// 쉐이더 버전
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&pixelShaderBuffer,				// 쉐이더가 컴파일 될 버퍼
		&errorMessage					// 에러 문자열
	);
	if (FAILED(result))
	{
		// 셰이더 컴파일이 실패하면 에러 메세지를 기록합니다.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hWnd, psFilename);
		}
		// 에러 메세지가 없다면 단순히 셰이더 파일을 찾지 못한 것입니다.
		else
		{
			MessageBox(hWnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// 버텍스, 픽셀 쉐이더가 잘 컴파일되면 쉐이더 버퍼가 생성되는데
	// 이를 이용하여 device로부터 쉐이더 객체를 생성한다
	// 버퍼로부터 버텍스 쉐이더 객체 생성
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	// 버퍼로부터 픽셀 쉐이더를 생성
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// 쉐이더에서 사용할 버텍스 데이터의 레이아웃(데이터가 어떤 데이터인지 셋팅)을 생성한다
	// 컬러 쉐이더에서는 위치 벡터와 색상 벡터를 사용한다
	// 레이아웃에 각각의 벡터 크기를 포함하는 두 레이아웃을 만든다

	// 정점 입력 레이아웃 description을 작성
	// 이 설정은 ModelClass와 셰이더에 있는 VertexType와 일치해야 한다

	// SemanticName - 이 요소가 레이아웃에서 어떻게 사용되는지 알려준다
	// 버텍스 쉐이더에서 입력은 POSITION, COLOR이므로 2개 이름을 지정한다
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;

	// 위치값에는 x(R32),y(G32),z(B32)만 사용
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;

	// 버퍼에 데이터가 어떻게 배열되는가
	// 0부터 시작함
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;


	// 컬러 레이아웃
	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;

	// 컬러값에는 r,g,b,a 4가지 요소 사용
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;

	// 위치벡터 사용하고 다음 요소 시작지점을 자동으로 찾는다
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// 레이아웃 desc작성 완료 후 Direct3D 장치로 입력 레이아웃을 생성한다
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// 버텍스 입력 레이아웃을 생성
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// 더 이상 사용되지 않는 정점 셰이더 퍼버와 픽셀 셰이더 버퍼를 해제
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// 쉐이더 사용을 위한 마지막 단계는 상수 버퍼이다
	// 쉐이더의 전역변수인 월드, 뷰, 투영행렬 버퍼 셋팅

	// 정점 셰이더에 있는 행렬 상수 버퍼의 description을 작성
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);

	// 상수 버퍼에 이 버퍼를 사용한다(bind, cpu 접근)는 것을 설정한다
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// 최종적으로 상수 버퍼 포인터를 만들어 이 클래스에서
	// 버텍스 셰이더 상수 버퍼에 접근할 수 있게 한다
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// 에러 메세지를 담고 있는 문자열 버퍼의 포인터를 가져옵니다.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// 메세지의 길이를 가져옵니다.
	bufferSize = errorMessage->GetBufferSize();

	// 파일을 열고 안에 메세지를 기록합니다.
	fout.open("shader-error.txt");

	// 에러 메세지를 씁니다.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// 파일을 닫습니다.
	fout.close();

	// 에러 메세지를 반환합니다.
	errorMessage->Release();
	errorMessage = nullptr;

	// 컴파일 에러가 있음을 팝업 메세지로 알려줍니다.
	MessageBox(hWnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

// 쉐이더의 전역 변수를 쉽게 다룰 수 있게 한다
// RenderManager에서 만들어진 행렬들을 사용한다
bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// transpose - 전치, 행과 열을 교환한다
	// 행렬을 transpose하여 쉐이더에서 사용할 수 있게 한다
	// 쉐이더에서나 directX -> openGL로 넘어갈 경우에도 전치를 사용하는 듯 - 서로 좌표계가 다르다

	// 쉐이더에서 전치행렬을 사용하는 이유
	// 쉐이더의 변환 매크로, 변환 명령들이 열 주도 행렬들에 적합하기 때문
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// 상수 버퍼의 내용을 쓸 수 있도록 잠근다
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// 상수 버퍼의 데이터에 대한 포인터를 가져온다
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// 상수 버퍼에 입력받은 행렬을 복사한다
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// 상수 버퍼의 잠금을 푼다
	deviceContext->Unmap(m_matrixBuffer, 0);

	// 버텍스 쉐이더에서의 상수 버퍼의 위치를 설정한다
	bufferNumber = 0;

	// 마지막으로 버텍스 쉐이더의 상수 버퍼를 바뀐 값으로 바꾼다
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}

// SetShaderParameters보다 먼저 호출되어 쉐이더의 인자들을 올바르게 셋팅한다
// 1. 입력 레이아웃을 Input assembler에 연결한다
// 이 연결로 GPU 버텍스 버퍼의 자료구조를 알게 된다
// 2. 버텍스 버퍼를 그리기 위한 버텍스, 픽셀 쉐이더를 설정한다
// 쉐이더가 설정되면 deviceContext에서 DrawIndexed 메소드로 삼각형을 그린다
void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// 1. 버텍스 입력 레이아웃을 설정한다
	// 해당 형태를 가진 입력을 받을 것이다는 셋팅
	deviceContext->IASetInputLayout(m_layout);

	// 2. 삼각형을 그릴 버텍스, 픽셀 쉐이더를 설정한다
	// 초기화 단계에서 만들었던 쉐이더를 가져온다
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// 삼각형을 그린다
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}

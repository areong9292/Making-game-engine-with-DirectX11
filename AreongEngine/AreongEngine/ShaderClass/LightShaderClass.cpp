#include "../stdafx.h"
#include "LightShaderClass.h"

LightShaderClass::LightShaderClass()
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_layout = nullptr;
	m_matrixBuffer = nullptr;
}

// ���ؽ� ���̴�, �ȼ� ���̴� ���õ��� ��ȯ�Ѵ�
LightShaderClass::~LightShaderClass()
{
	// ����Ʈ ���� ����
	if (m_lightBuffer != nullptr)
	{
		m_lightBuffer->Release();
		m_lightBuffer = nullptr;
	}

	// ���÷� ���� ����
	if (m_sampleState != nullptr)
	{
		m_sampleState->Release();
		m_sampleState = nullptr;
	}

	// ��� ���۸� �����Ѵ�
	if (m_matrixBuffer != nullptr)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}

	// ���̾ƿ��� �����Ѵ�
	if (m_layout != nullptr)
	{
		m_layout->Release();
		m_layout = nullptr;
	}

	// �ȼ� ���̴��� �����Ѵ�
	if (m_pixelShader != nullptr)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	// ���� ���̴��� �����Ѵ�
	if (m_vertexShader != nullptr)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}

	MessageBox(0, L"~LightShaderClass", L"Error", MB_OK);
}

bool LightShaderClass::Initialize(ID3D11Device* device, HWND hWnd)
{
	bool result;

	// ���̴��� �ʱ�ȭ�ϴ� �޼ҵ�
	// HLSL ���̴� ������ �̸��� �Ѱ��ش�
	result = InitializeShader(device, hWnd, L"./HLSL/Light.vs", L"./HLSL/Light.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

bool LightShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
	bool result;

	// �������� ����� ���̴��� ���ڸ� �Է��Ѵ�
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, diffuseColor);
	if (!result)
	{
		return false;
	}

	// ���̴��� �غ�� ���۸� �׸���
	RenderShader(deviceContext, indexCount);

	return true;
}

// ���̴� ������ �ҷ����� DirectX�� GPU���� ��� �����ϵ��� �Ѵ�
bool LightShaderClass::InitializeShader(ID3D11Device* device, HWND hWnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;

	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	// �� ���̴����� �Է� ��Ҹ� 3��(position, texture, color)�� ����Ѵ�
	// �Է� ��� ���Ϳ� ���� 3���� �Է� ���̾ƿ� ������ �ʿ��ϴ�
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];

	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	D3D11_SAMPLER_DESC samplerDesc;

	D3D11_BUFFER_DESC lightBufferDesc;

	// �� �Լ����� ����ϴ� �����͵��� null�� ����
	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	// ���̴� ���α׷��� ���۷� �������Ѵ�
	// ���� ���̴��� �������Ѵ�
	result = D3DCompileFromFile(
		vsFilename,						// ���̴� ���� �̸�
		NULL,
		NULL,
		"LightVertexShader",			// ���̴� �̸�
		"vs_5_0",						// ���̴� ����
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&vertexShaderBuffer,			// ���̴��� ������ �� ����
		&errorMessage					// ���� ���ڿ�
	);

	if (FAILED(result))
	{
		// ���̴��� �����Ͽ� �����ϸ� ���� �޼����� ���
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hWnd, vsFilename);
		}
		// ���� �޼����� ���ٸ� ���̴� ������ ã�� ���� ��
		else
		{
			MessageBox(hWnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// �ȼ� ���̴��� �������Ѵ�
	result = D3DCompileFromFile(
		psFilename,						// ���̴� ���� �̸�
		NULL,
		NULL,
		"LightPixelShader",				// ���̴� �̸�
		"ps_5_0",						// ���̴� ����
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&pixelShaderBuffer,				// ���̴��� ������ �� ����
		&errorMessage					// ���� ���ڿ�
	);
	if (FAILED(result))
	{
		// ���̴� �������� �����ϸ� ���� �޼����� ����մϴ�.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hWnd, psFilename);
		}
		// ���� �޼����� ���ٸ� �ܼ��� ���̴� ������ ã�� ���� ���Դϴ�.
		else
		{
			MessageBox(hWnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// ���ؽ�, �ȼ� ���̴��� �� �����ϵǸ� ���̴� ���۰� �����Ǵµ�
	// �̸� �̿��Ͽ� device�κ��� ���̴� ��ü�� �����Ѵ�
	// ���۷κ��� ���ؽ� ���̴� ��ü ����
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	// ���۷κ��� �ȼ� ���̴��� ����
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// ���̴����� ����� ���ؽ� �������� ���̾ƿ�(�����Ͱ� � ���������� ����)�� �����Ѵ�
	// �÷� ���̴������� ��ġ ���Ϳ� ���� ���͸� ����Ѵ�
	// ���̾ƿ��� ������ ���� ũ�⸦ �����ϴ� �� ���̾ƿ��� �����

	// ���� �Է� ���̾ƿ� description�� �ۼ�
	// �� ������ ModelClass�� ���̴��� �ִ� VertexType�� ��ġ�ؾ� �Ѵ�

	// SemanticName - �� ��Ұ� ���̾ƿ����� ��� ���Ǵ��� �˷��ش�
	// ���ؽ� ���̴����� �Է��� POSITION, TEXCOORD, NORMAL�̹Ƿ� 3�� �̸��� �����Ѵ�
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;

	// ��ġ������ x(R32),y(G32),z(B32)�� ���
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;

	// ���ۿ� �����Ͱ� ��� �迭�Ǵ°�
	// 0���� ������
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;


	// �ؽ��� ���̾ƿ�
	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;

	// �ؽ��� ������ r,g 2���� ��� ���
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;

	// ��ġ���� ����ϰ� ���� ��� ���������� �ڵ����� ã�´�
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// ����Ʈ ���̾ƿ�
	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;

	// �ؽ��� ������ r,g,b 2���� ��� ���
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;

	// ��ġ���� ����ϰ� ���� ��� ���������� �ڵ����� ã�´�
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// ���̾ƿ� desc�ۼ� �Ϸ� �� Direct3D ��ġ�� �Է� ���̾ƿ��� �����Ѵ�
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// ���ؽ� �Է� ���̾ƿ��� ����
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// �� �̻� ������ �ʴ� ���� ���̴� �۹��� �ȼ� ���̴� ���۸� ����
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// ���̴� ����� ���� ������ �ܰ�� ��� �����̴�
	// ���̴��� ���������� ����, ��, ������� ���� ����

	// ���� ���̴��� �ִ� ��� ��� ������ description�� �ۼ�
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);

	// ��� ���ۿ� �� ���۸� ����Ѵ�(bind, cpu ����)�� ���� �����Ѵ�
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// ���������� ��� ���� �����͸� ����� �� Ŭ��������
	// ���ؽ� ���̴� ��� ���ۿ� ������ �� �ְ� �Ѵ�
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// �ؽ�ó ���÷� ���� ����ü�� ���� �� �����Ѵ�
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// �ؽ�ó ���÷� ������Ʈ ����
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	// ����Ʈ ���� ���� ����ü ���� �� ����
	// D3D11_BIND_CONSTANT_BUFFER�� ��� ���۰� �׻� 16�� ������� �� - �ȱ׷��� CreateBuffer ������
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBuffertype);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// ���� �޼����� ��� �ִ� ���ڿ� ������ �����͸� �����ɴϴ�.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// �޼����� ���̸� �����ɴϴ�.
	bufferSize = errorMessage->GetBufferSize();

	// ������ ���� �ȿ� �޼����� ����մϴ�.
	fout.open("shader-error.txt");

	// ���� �޼����� ���ϴ�.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// ������ �ݽ��ϴ�.
	fout.close();

	// ���� �޼����� ��ȯ�մϴ�.
	errorMessage->Release();
	errorMessage = nullptr;

	// ������ ������ ������ �˾� �޼����� �˷��ݴϴ�.
	MessageBox(hWnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

// ���̴��� ���� ������ ���� �ٷ� �� �ְ� �Ѵ�
// RenderManager���� ������� ��ĵ��� ����Ѵ�
bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBuffertype* dataPtr2;
	unsigned int bufferNumber;

	// transpose - ��ġ, ��� ���� ��ȯ�Ѵ�
	// ����� transpose�Ͽ� ���̴����� ����� �� �ְ� �Ѵ�
	// ���̴������� directX -> openGL�� �Ѿ ��쿡�� ��ġ�� ����ϴ� �� - ���� ��ǥ�谡 �ٸ���

	// ���̴����� ��ġ����� ����ϴ� ����
	// ���̴��� ��ȯ ��ũ��, ��ȯ ��ɵ��� �� �ֵ� ��ĵ鿡 �����ϱ� ����
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// ��� ������ ������ �� �� �ֵ��� ��ٴ�
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// ��� ������ �����Ϳ� ���� �����͸� �����´�
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// ��� ���ۿ� �Է¹��� ����� �����Ѵ�
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// ��� ������ ����� Ǭ��
	deviceContext->Unmap(m_matrixBuffer, 0);

	// ���ؽ� ���̴������� ��� ������ ��ġ�� �����Ѵ�
	bufferNumber = 0;

	// ���������� ���ؽ� ���̴��� ��� ���۸� �ٲ� ������ �ٲ۴�
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// �ȼ� ���̴��� �ؽ�ó ���ҽ� ����
	deviceContext->PSSetShaderResources(0, 1, &texture);



	// ����Ʈ ������ ������ �� �� �ֵ��� ��ٴ�
	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// ����Ʈ ������ �����Ϳ� ���� �����͸� �����´�
	dataPtr2 = (LightBuffertype*)mappedResource.pData;

	// ����Ʈ ���ۿ� �Է� ���� �� ������ �����Ѵ�
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->padding = 0.0f;

	// ����Ʈ ������ ����� Ǭ��
	deviceContext->Unmap(m_lightBuffer, 0);

	// ���ؽ� ���̴������� ��� ������ ��ġ�� �����Ѵ�
	bufferNumber = 0;

	// ���������� ����Ʈ�� �ȼ� ���̴����� ó���ϹǷ�
	// �ȼ� ���̴��� ����Ʈ ���۸� �ٲ� ������ �ٲ۴�
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	return true;
}

// SetShaderParameters���� ���� ȣ��Ǿ� ���̴��� ���ڵ��� �ùٸ��� �����Ѵ�
// 1. �Է� ���̾ƿ��� Input assembler�� �����Ѵ�
// �� ����� GPU ���ؽ� ������ �ڷᱸ���� �˰� �ȴ�
// 2. ���ؽ� ���۸� �׸��� ���� ���ؽ�, �ȼ� ���̴��� �����Ѵ�
// ���̴��� �����Ǹ� deviceContext���� DrawIndexed �޼ҵ�� �ﰢ���� �׸���
void LightShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// 1. ���ؽ� �Է� ���̾ƿ��� �����Ѵ�
	// �ش� ���¸� ���� �Է��� ���� ���̴ٴ� ����
	deviceContext->IASetInputLayout(m_layout);

	// 2. �ﰢ���� �׸� ���ؽ�, �ȼ� ���̴��� �����Ѵ�
	// �ʱ�ȭ �ܰ迡�� ������� ���̴��� �����´�
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// �ȼ� ���̴����� ���÷� ���¸� �����Ѵ�
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// �ﰢ���� �׸���
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}

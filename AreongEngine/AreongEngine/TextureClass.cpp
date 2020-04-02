#include "stdafx.h"
#include "TextureClass.h"

TextureClass::TextureClass()
{
	m_textureView = nullptr;
	m_texture = nullptr;
	m_targaData = nullptr;
}

TextureClass::~TextureClass()
{
	// Release the texture view resource.
	if (m_textureView != nullptr)
	{
		m_textureView->Release();
		m_textureView = nullptr;
	}

	// Release the texture.
	if (m_texture != nullptr)
	{
		m_texture->Release();
		m_texture = nullptr;
	}

	// Release the targa data.
	if (m_targaData != nullptr)
	{
		delete[] m_targaData;
		m_targaData = nullptr;
	}
}

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	// Load the texture in.
	ScratchImage image = LoadTextureFromFile(filename);
	result = CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &m_textureView);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView * TextureClass::GetTexture()
{
	return m_textureView;
}

ScratchImage TextureClass::LoadTextureFromFile(WCHAR* filename)
{
	wstring wcharTowstring(filename);
	string resultstr(wcharTowstring.begin(), wcharTowstring.end());

	// Load the texture.
	string str(resultstr);
	wstring wsTmp(str.begin(), str.end());

	wstring ws = wsTmp;
	// Load the texture.
	WCHAR ext[_MAX_EXT];
	_wsplitpath_s(ws.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

	HRESULT hr;
	ScratchImage image;
	if (_wcsicmp(ext, L".dds") == 0)
	{
		hr = LoadFromDDSFile(ws.c_str(), DDS_FLAGS_NONE, nullptr, image);
	}

	else if (_wcsicmp(ext, L".tga") == 0)
	{
		hr = LoadFromTGAFile(ws.c_str(), nullptr, image);
	}

	else if (_wcsicmp(ext, L".wic") == 0)
	{
		hr = LoadFromWICFile(ws.c_str(), WIC_FLAGS_NONE, nullptr, image);
	}

	return image;
}
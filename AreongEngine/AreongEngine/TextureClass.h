#pragma once

#include "stdio.h"
#include "DirectXTex.h"
using namespace std;

// 텍스쳐 자원을 불러오고 해제하고 접근하는 작업
// 모든 텍스쳐에 대해 각각 이 클래스가 만들어져야한다
class TextureClass
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	TextureClass();
	~TextureClass();

	// 파일 이름으로 텍스쳐를 불러온다
	bool Initialize(ID3D11Device* device, WCHAR* filename);

	// 텍스쳐 리소스 포인터를 반환
	ID3D11ShaderResourceView* GetTexture();

private:
	ScratchImage LoadTextureFromFile(WCHAR* filename);

private:
	// 텍스쳐 리소스 포인터
	ID3D11ShaderResourceView* m_textureView;
	ID3D11Texture2D* m_texture;
	unsigned char* m_targaData;
};

#pragma once

#include "stdio.h"
#include "DirectXTex.h"
using namespace std;

// �ؽ��� �ڿ��� �ҷ����� �����ϰ� �����ϴ� �۾�
// ��� �ؽ��Ŀ� ���� ���� �� Ŭ������ ����������Ѵ�
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

	// ���� �̸����� �ؽ��ĸ� �ҷ��´�
	bool Initialize(ID3D11Device* device, WCHAR* filename);

	// �ؽ��� ���ҽ� �����͸� ��ȯ
	ID3D11ShaderResourceView* GetTexture();

private:
	ScratchImage LoadTextureFromFile(WCHAR* filename);

private:
	// �ؽ��� ���ҽ� ������
	ID3D11ShaderResourceView* m_textureView;
	ID3D11Texture2D* m_texture;
	unsigned char* m_targaData;
};

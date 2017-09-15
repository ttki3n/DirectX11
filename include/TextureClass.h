#pragma once

#include "D3DHelper.h"
#include "DirectXTex.h"

class TextureClass
{
private:
	struct TargaHeader
	{
		unsigned char header[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	TextureClass();
	TextureClass(const TextureClass& other);
	~TextureClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadTarga(char*, std::unique_ptr<ScratchImage> &img);
	
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
};

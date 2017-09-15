#include "stdafx.h"

#include "TextureClass.h"

TextureClass::TextureClass()
{
	m_targaData = 0;
	m_texture = 0;
	m_textureView = 0;
}

TextureClass::TextureClass(const TextureClass& other)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;
	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hr;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	// Load the targa image data into memory
	result = LoadTarga(filename, width, height);
	if (!result)
	{
		return false;
	}

	// Setup the description of the texture
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create teh empty texture
	hr = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hr))
	{
		return false;
	}

	// Set the row pitch of the targa image data
	rowPitch = (width * 4) * sizeof(unsigned char);

	// Copy the targe image data into the texture
	deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

	// Setup the shader resource view description
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture
	hr = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if (FAILED(hr))
	{
		return false;
	}

	// Generate mipmaps for this texture
	deviceContext->GenerateMips(m_textureView);

	//delete m_targaData;
	//m_targaData = 0;

	return true;
}

void TextureClass::Shutdown()
{
	COM_SAFE_RELEASE(m_textureView);
	COM_SAFE_RELEASE(m_texture);
	
	if (m_targaData)
	{
		//delete m_targaData;
		//m_targaData = 0;
	}
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView;
}

bool TextureClass::LoadTarga(char* filename, int&width, int&height)
{
	ScratchImage *image = new ScratchImage();

	size_t length = strlen(filename);
	std::wstring wc(length, L'#');
	mbstowcs(&wc[0], filename, length);
	
	HRESULT hr = LoadFromTGAFile(wc.c_str(), nullptr, *image);
	if (FAILED(hr))
	{
		return false;
	}

	const Image *img = image->GetImage(0, 0, 0);
	width = img->width;
	height = img->height;
	m_targaData = img->pixels;

	return true;
}

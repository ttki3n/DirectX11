#include "stdafx.h"

#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename)
{
	bool result;
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	// Set triangle manually for this tutorial
	VertexType* vertices = nullptr;
	unsigned long* indices = nullptr;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT hr;

	//LoadSampleTriangle(&vertices, m_vertexCount, &indices, m_indexCount);
	LoadSampleCube(&vertices, m_vertexCount, &indices, m_indexCount);
	
	// Setup the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	// Release the arrays 
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
	
	return true;
}

void ModelClass::ShutdownBuffers()
{
	COM_SAFE_RELEASE(m_indexBuffer);
	COM_SAFE_RELEASE(m_vertexBuffer);
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active inthe input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;

	m_texture = new TextureClass();
	if (!m_texture)
	{
		return false;
	}

	// Initialize the texture object
	result = m_texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}
}

bool ModelClass::LoadSampleTriangle(VertexType **vertices, int &vertexCount, unsigned long **indices, int &indexCount)
{
	// Triangle data	
	vertexCount = 3;
	indexCount = 3;

	*vertices = new VertexType[vertexCount];
	if (!*vertices)
	{
		return false;
	}

	*indices = new unsigned long[indexCount];
	if (!*indices)
	{
		return false;
	}

	// Sample data
	(*vertices)[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	(*vertices)[0].texcoord = XMFLOAT2(0.0f, 1.0f);
	(*vertices)[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	(*vertices)[1].texcoord = XMFLOAT2(0.5f, 0.0f);
	(*vertices)[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	(*vertices)[2].texcoord = XMFLOAT2(1.0f, 1.0f);

	// Load the index array with data
	(*indices)[0] = 0;
	(*indices)[1] = 1;
	(*indices)[2] = 2;

	return true;
}

bool ModelClass::LoadSampleCube(VertexType **vertices, int &vertexCount, unsigned long **indices, int &indexCount)
{
	vertexCount = 8;
	indexCount = 36;
	
	*vertices = new VertexType[vertexCount]
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) }, // 0
		{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) }, // 1
		{ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) }, // 2
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) }, // 3
		{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT2(1.0f, 1.0f) }, // 4
		{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT2(1.0f, 0.0f) }, // 5
		{ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) }, // 6
		{ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT2(0.0f, 0.0f) }  // 7
	};
	
	*indices = new unsigned long[indexCount]
	{
		0, 1, 2, 0, 2, 3,
		4, 6, 5, 4, 7, 6,
		4, 5, 1, 4, 1, 0,
		3, 2, 6, 3, 6, 7,
		1, 5, 6, 1, 6, 2,
		4, 0, 3, 4, 3, 7
	};

	return true;
}
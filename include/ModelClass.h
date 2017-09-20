#pragma once

#include "D3DHelper.h"
#include "TextureClass.h"

class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texcoord;
		XMFLOAT3 normal;
	};

public:
	ModelClass();
	ModelClass(const ModelClass& other);
	~ModelClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	bool LoadSampleTriangle(VertexType **vertices, int &vertexCount, unsigned long **indices, int &indexCount);
	bool LoadSampleCube(VertexType **vertices, int &vertexCount, unsigned long **indices, int &indexCount);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_texture;
};
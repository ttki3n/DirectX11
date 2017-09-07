#include "stdafx.h"

#include "ColorShaderClass.h"

ColorShaderClass::ColorShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& other)
{
}

ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// Initialize the vertex and pixel shaders
	result = InitializeShader(device, hwnd, L"../src/shaders/colorVS.hlsl", L"../src/shaders/colorPS.hlsl");
	if (!result)
	{
		return false;
	}
	return true;
}

void ColorShaderClass::Shutdown()
{
	ShutdownShader();
}

bool ColorShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;

	// Set the shader parameters that it will use for rendering
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);
	return true;
}

bool ColorShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT hr;
	ID3DBlob* errorMsg;
	ID3DBlob* vsBlob;
	ID3DBlob* psBlob;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// Initialize the pointers this function will use to NULL
	errorMsg = 0;
	vsBlob = 0;
	psBlob = 0;

	// Compile the vertex shader code
	hr = D3DCompileFromFile(vsFilename, NULL, NULL, "ColorVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &vsBlob, &errorMsg);
	if (FAILED(hr))
	{
		if (errorMsg)
		{
			OutputShaderErrorMessage(errorMsg, hwnd, vsFilename);
		}
		else // If there was nothing in the error message then it simply could not find the shader file
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	// Compile the pixel shader code
	hr = D3DCompileFromFile(psFilename, NULL, NULL, "ColorPS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &psBlob, &errorMsg);
	if (FAILED(hr))
	{
		if (errorMsg)
		{
			OutputShaderErrorMessage(errorMsg, hwnd, psFilename);
		}
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	// Create the vertex shader and pixel shader from the buffers
	hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(hr))
	{
		return false;
	}
	
	hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(hr))
	{
		return false;
	}

	// Create the vertex input layout description
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout
	hr = device->CreateInputLayout(polygonLayout, numElements, vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(), &m_layout);
	if (FAILED(hr))
	{
		return false;
	}

	COM_SAFE_RELEASE(vsBlob);
	COM_SAFE_RELEASE(psBlob);

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	hr = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	
	return true;
}

void ColorShaderClass::ShutdownShader()
{
	// Release interfaces that were setup in InitializeShader
	COM_SAFE_RELEASE(m_matrixBuffer);
	COM_SAFE_RELEASE(m_layout);
	COM_SAFE_RELEASE(m_pixelShader);
	COM_SAFE_RELEASE(m_vertexShader);
}

void ColorShaderClass::OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	std::ofstream outFile;
	
	compileErrors = (char*)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();

	outFile.open("shader-error.txt");
	for (i = 0; i < bufferSize; ++i)
	{
		outFile << compileErrors[i];
	}
	outFile.close();

	// Release the error msg
	COM_SAFE_RELEASE(errorMessage);

	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt", shaderFilename, MB_OK);
}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedRes;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the m_matrixBuffer, set new matrices inside it, then unlock it
	hr = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
	if (FAILED(hr))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer
	dataPtr = (MatrixBufferType*)mappedRes.pData;
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Render the triangle
	deviceContext->DrawIndexed(indexCount, 0, 0);

}
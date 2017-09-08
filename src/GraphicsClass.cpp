#include "stdafx.h"
#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_directX3DPtr = nullptr;
	m_cameraPtr = nullptr;
	m_modelPtr = nullptr;
	m_colorShaderPtr = nullptr;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(unsigned int width, unsigned int height, HWND hwnd)
{
	bool result;
	m_directX3DPtr = std::make_unique<D3DClass>();
	if (!m_directX3DPtr)
	{
		return false;
	}

	result = m_directX3DPtr->Initialize(width, height, IS_VSYNC_ENABLED, hwnd, IS_FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, TEXT("Couldn't initiliaze DirectX"), TEXT("Error"), MB_OK);
		return false;
	}

	m_cameraPtr = std::make_unique<CameraClass>();
	if (!m_cameraPtr)
	{
		return false;
	}

	m_cameraPtr->SetPosition(0.0f, 0.0f, -5.0f);

	m_modelPtr = std::make_unique<ModelClass>();
	if (!m_modelPtr)
	{
		return false;
	}

	result = m_modelPtr->Initialize(m_directX3DPtr->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	m_colorShaderPtr = std::make_unique<ColorShaderClass>();
	if (!m_colorShaderPtr)
	{
		return false;
	}

	result = m_colorShaderPtr->Initialize(m_directX3DPtr->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_colorShaderPtr)
	{
		m_colorShaderPtr->Shutdown();
	}

	if (m_modelPtr)
	{
		m_modelPtr->Shutdown();
	}	

	// Release the DirectX3D object
	if (m_directX3DPtr)
	{
		m_directX3DPtr->Shutdown();
	}
}

bool GraphicsClass::Frame(float deltaTime)
{
	bool result;
	result = Render(deltaTime);
	return result;
}

bool GraphicsClass::Render(float deltaTime)
{	
	static float angle = 0.0f;
	static XMVECTOR rotationAxis = XMVectorSet(0, 1, 1, 0);

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	m_directX3DPtr->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// Generate the view matrix based on the camera's position
	m_cameraPtr->Render();

	// Get the world, view and projection matrices from the camera & d3d object
	//m_directX3DPtr->GetWorldMatrix(worldMatrix);
	m_directX3DPtr->GetProjectionMatrix(projectionMatrix);
	m_cameraPtr->GetViewMatrix(viewMatrix);
		
	angle += 1.0f * deltaTime;
	worldMatrix = XMMatrixRotationAxis(rotationAxis, angle);
	
	// Put the model on the graphics pipeline to prepate them for drawing
	m_modelPtr->Render(m_directX3DPtr->GetDeviceContext());

	// Render the model using the color shader
	result = m_colorShaderPtr->Render(m_directX3DPtr->GetDeviceContext(), m_modelPtr->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	m_directX3DPtr->EndScene();
	
	return true;
}



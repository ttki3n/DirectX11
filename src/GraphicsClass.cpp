#include "stdafx.h"
#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_directX3DPtr = nullptr;
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

	return true;
}

void GraphicsClass::Shutdown()
{
	// Release the DirectX3D object
	if (m_directX3DPtr)
	{
		m_directX3DPtr->Shutdown();
	}
}

bool GraphicsClass::Frame()
{
	bool result;
	result = Render();
	return result;
}

bool GraphicsClass::Render()
{
	static float r_color = 0.0f;
	m_directX3DPtr->BeginScene(r_color, 0.5f, 0.5f, 1.0f);

	m_directX3DPtr->EndScene();
	r_color += 0.01f;
	if (r_color > 1.0f)
	{
		r_color = 0.0f;
	}
	return true;
}



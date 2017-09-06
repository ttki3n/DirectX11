#pragma once

// Link directX lib
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

// Helper functions
#define COM_SAFE_RELEASE(obj)\
{\
	if (obj != NULL)\
	{\
		obj->Release();\
		obj = 0;\
	}\
}

bool QueryRefreshRate(unsigned int screenWidth, unsigned int screenHeight, bool vsync, DXGI_RATIONAL& refreshRate);
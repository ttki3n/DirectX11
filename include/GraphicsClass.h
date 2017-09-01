#pragma once

#include "D3DClass.h"

const bool IS_FULL_SCREEN = false;
const bool IS_VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass& other);
	
	~GraphicsClass();

	bool Initialize(unsigned int width, unsigned int height, HWND hwnd);
	void Shutdown();
	bool Frame();

private:
	bool Render();
	std::unique_ptr<D3DClass> m_directX3DPtr;

};
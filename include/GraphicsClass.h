#pragma once

#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "TextureShaderClass.h"

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
	bool Frame(float deltaTime);

private:
	bool Render(float deltaTime);
	std::unique_ptr<D3DClass> m_directX3DPtr;
	std::unique_ptr<CameraClass> m_cameraPtr;
	std::unique_ptr<ModelClass> m_modelPtr;
	std::unique_ptr<TextureShaderClass> m_shaderPtr;
};
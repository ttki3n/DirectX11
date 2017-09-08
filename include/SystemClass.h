#pragma once

#include "InputClass.h"
#include "GraphicsClass.h"
#include <chrono>

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

class SystemClass;
static SystemClass* ApplicationHandle = 0;


class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass& other);
	
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT, WPARAM, LPARAM);

private:
	bool Frame(float deltaTime);
	void InitializeWindows(int& screenWidth, int& screenHeight);
	void ShutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;
	
	std::chrono::time_point<std::chrono::steady_clock> m_prevTime, m_curTime;
	float m_deltaTime;
	float m_targetFramerate, m_maxTimeStep;

	std::unique_ptr<InputClass> m_Input;
	std::unique_ptr<GraphicsClass> m_Graphics;

	
};

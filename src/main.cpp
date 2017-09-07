#include "stdafx.h"

#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	std::unique_ptr<SystemClass> system = std::make_unique<SystemClass>();
	if (!system)
	{
		return 0;
	}

	bool result;
	result = system->Initialize();
	if (result)
	{
		system->Run();
	}

	// Shutdown and release the system object
	system->Shutdown();

	return 1;
}
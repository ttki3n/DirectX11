#include "stdafx.h"
#include "InputClass.h"

InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	// Initialize all the keys to being released
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}
}

void InputClass::KeyDown(unsigned int key)
{
	m_keys[key] = true;
}

void InputClass::KeyUp(unsigned int key)
{
	m_keys[key] = false;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}
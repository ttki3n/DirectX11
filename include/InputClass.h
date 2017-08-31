#pragma once

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass& other);
	
	~InputClass();

	void Initialize();

	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);
	bool IsKeyDown(unsigned int key);

private:
	bool m_keys[256];
};

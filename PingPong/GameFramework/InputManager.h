#pragma once

#include <iostream>

class InputManager
{
public:
	static InputManager* Instance;

	InputManager();
	~InputManager();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool Keys[256];
};


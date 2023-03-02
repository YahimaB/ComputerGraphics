#pragma once

#include <iostream>
#include "Keys.h"

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
	bool IsKeyDown(Keys key);

private:
	bool keys[256];
};


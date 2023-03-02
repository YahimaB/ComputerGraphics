#include "InputManager.h"

InputManager* InputManager::Instance = 0;

InputManager::InputManager()
{
	Instance = this;
}

InputManager::~InputManager()
{
}

void InputManager::Initialize()
{
	for (int i = 0; i < 256; i++)
	{
		keys[i] = false;
	}

	return;
}

void InputManager::KeyDown(unsigned int input)
{
	std::cout << "Key pressed: " << input << std::endl;
	keys[input] = true;
	return;
}

void InputManager::KeyUp(unsigned int input)
{
	std::cout << "Key released: " << input << std::endl;
	keys[input] = false;
	return;
}

bool InputManager::IsKeyDown(unsigned int key)
{
	return keys[key];
}

bool InputManager::IsKeyDown(Keys key)
{
	return keys[(int) key];
}

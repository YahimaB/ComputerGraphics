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
		Keys[i] = false;
	}

	return;
}

void InputManager::KeyDown(unsigned int input)
{
	std::cout << "Key pressed: " << input << std::endl;
	Keys[input] = true;
	return;
}

void InputManager::KeyUp(unsigned int input)
{
	std::cout << "Key released: " << input << std::endl;
	Keys[input] = false;
	return;
}

bool InputManager::IsKeyDown(unsigned int key)
{
	return Keys[key];
}

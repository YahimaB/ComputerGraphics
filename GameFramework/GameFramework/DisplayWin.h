#pragma once

#include <windows.h>

class DisplayWin
{
public:
	float ClientHeight;
	float ClientWidth;

public:
	DisplayWin();
	~DisplayWin();


private:
	HWND hWnd;
	HINSTANCE hInstance;
	WNDCLASSEX wc;
};


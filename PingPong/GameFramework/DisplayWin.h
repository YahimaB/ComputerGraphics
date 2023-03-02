#pragma once

#include <windows.h>

class DisplayWin
{
public:
	LONG ClientHeight;
	LONG ClientWidth;
	HWND hWnd;

public:
	DisplayWin(LPCWSTR appName);
	~DisplayWin();


private:
	HINSTANCE hInstance;
	WNDCLASSEX wc = {};
};


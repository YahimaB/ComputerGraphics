#pragma once

#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include <chrono>

#include <d3d.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include "DisplayWin.h"


class Game
{
public:
	DisplayWin* Display;

public:
	Game(LPCWSTR appName);
	~Game();

	bool Initialize();
	void Run();
	void Update();
	void Exit();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);


private:
	LPCWSTR name;
	std::chrono::steady_clock::time_point StartTime;
	std::chrono::steady_clock::time_point PrevTime;

	float TotalTime;
	unsigned int FrameCount;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;

	ID3D11RenderTargetView* renderView;
	ID3D11RasterizerState* rastState;
	D3D11_VIEWPORT viewport;

	ID3D11Debug* debug;
	ID3DUserDefinedAnnotation* annotation;

private:
	bool CreateBackBuffer();
	void UpdateInternal();
	void PrepareFrame();
	void EndFrame();
	void Draw();
};

static Game* Instance = 0;

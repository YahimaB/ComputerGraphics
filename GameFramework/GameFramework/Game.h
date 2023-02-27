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

class Game
{
public:
	static Game *Instanse;

public:
	Game();
	~Game();

	bool Initialize();
	bool Run();
	bool Update();
	bool Exit();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);


private:
	LPCWSTR name;
	std::chrono::steady_clock::time_point StartTime;
	std::chrono::steady_clock::time_point TotalTime;
	std::chrono::steady_clock::time_point PrevTime;

	HRESULT res;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;

	ID3D11RenderTargetView* renderView;
	ID3D11RasterizerState* rastState;
	D3D11_VIEWPORT viewport;

	ID3D11Debug* debug;
	ID3DUserDefinedAnnotation* annotation;

private:
	bool UpdateInternal();

};


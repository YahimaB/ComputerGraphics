#pragma once

#include "MinimalCore.h"

#include "DisplayWin.h"
#include "ShaderManager.h"
#include "GameComponent.h"
#include "Components/Camera.h"
#include "Components/CameraController.h"
#include "External/InputDevice.h"


class Game
{
public:
	static Game* Instance;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

public:
	DisplayWin* Display;
	ShaderManager* Shader;
	InputDevice* InputDevice;
	Camera* Camera;
	CameraController* controller;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;

	ID3D11RenderTargetView* renderView;
	ID3D11DepthStencilView* depthView;
	ID3D11RasterizerState* rastState;
	D3D11_VIEWPORT viewport;

	int componentsCount = 0;
	static const int MAX_COMPONENTS = 500;
	GameComponent* components_[MAX_COMPONENTS];

public:
	Game(LPCWSTR appName);
	~Game();

	void CreateComponent(GameComponent* component);
	bool Initialize();
	void Run();
	void Update(float deltaTime);
	void Exit();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);


private:
	LPCWSTR name;
	std::chrono::steady_clock::time_point StartTime;
	std::chrono::steady_clock::time_point PrevTime;

	float TotalTime;
	unsigned int FrameCount;

	

	ID3D11Debug* debug;
	ID3DUserDefinedAnnotation* annotation;

private:
	bool CreateBackBuffer();
	void UpdateInternal();
	void PrepareFrame();
	void EndFrame();
	void Draw();
};
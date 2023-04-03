#pragma once

#include "MinimalCore.h"

#include "DisplayWin.h"
#include "Managers/ShaderManager.h"
#include "External/InputDevice.h"

class GameObject;

class Game
{
public:
	static Game* Instance;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

public:
	DisplayWin* Display;
	D3D11_VIEWPORT viewport;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;

	ID3D11RenderTargetView* mainRTV;
	ID3D11DepthStencilView* mainDSV;

	ID3D11DepthStencilView* shadowDSV;
	ID3D11ShaderResourceView* shadowSRV;

	ID3D11RasterizerState* rastState;

public:
	ShaderManager* ShaderManager;
	InputDevice* InputDevice;

public:
	Game(LPCWSTR appName);
	~Game();

	bool Initialize();
	void Run();
	void Update(float deltaTime);
	void Exit();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

	void AddGameObject(GameObject* gameObject);
	std::vector<GameObject*> FindGameObjects(std::string name);

private:
	LPCWSTR name;
	std::chrono::steady_clock::time_point StartTime;
	std::chrono::steady_clock::time_point PrevTime;

	float TotalTime;
	unsigned int FrameCount;

	ID3D11Debug* debug;
	ID3DUserDefinedAnnotation* annotation;

	std::vector<GameObject*> _gameObjects;

private:
	void UpdateInternal();
	void PrepareFrame();
	void EndFrame();
	void Draw();

private:
	void CreateDisplay();
	bool CreateResources();
	bool CreateDeviceAndSwapChain();
	bool CreateMainRenderResources();
	bool CreateShadowRenderResources();
	bool CreateRasterizerState();
	bool CreateShadowRasterizerState();
};
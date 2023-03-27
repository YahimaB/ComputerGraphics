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
	ShaderManager* ShaderManager;
	InputDevice* InputDevice;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;

	ID3D11RenderTargetView* renderView;
	ID3D11DepthStencilView* depthView;
	ID3D11RasterizerState* rastState;

	ID3D11Texture2D* shadowTexArr;
	ID3D11DepthStencilView* depthShadowDsv;
	ID3D11ShaderResourceView* depthShadowSrv;

	D3D11_VIEWPORT viewport;

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
	void CreateCsmDepthTextureArray();
	bool CreateBackBuffer();
	void UpdateInternal();
	void PrepareFrame();
	void EndFrame();
	void Draw();
};
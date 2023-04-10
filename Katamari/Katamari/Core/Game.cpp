#include "Game.h"
#include "GameObject.h"

#include <wincodec.h> // Optional
#include "External/ScreenGrab11.h"

const float MS_PER_UPDATE = 15;
Game* Game::Instance = 0;

Game::Game(LPCWSTR appName)
{
	if (Instance != 0)
	{
		throw "Trying to create another Game instance";
	}

	name = appName;
	TotalTime = 0;
	FrameCount = 0;

	Instance = this;
}

Game::~Game()
{
	SaveWICTextureToFile(context, backBufferTex, GUID_ContainerFormatJpeg, L"SCREENSHOT.JPG");

}

void Game::AddGameObject(GameObject* gameObject)
{
	_gameObjects.push_back(gameObject);
}

std::vector<GameObject*> Game::FindGameObjects(std::string name)
{
	std::vector<GameObject*> objects;

	for (auto gameObject : _gameObjects)
	{
		if (gameObject->Name.compare(name) == 0)
		{
			objects.push_back(gameObject);
		}
	}

	return objects;
}

void Game::Run()
{
	StartTime = std::chrono::steady_clock::now();
	PrevTime = std::chrono::steady_clock::now();

	float lag = 0;
	MSG msg = {};
	bool isExitRequested = false;
	while (!isExitRequested) {

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				isExitRequested = true;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		auto currentTime = std::chrono::steady_clock::now();
		auto deltaTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - PrevTime).count();
		PrevTime = currentTime;

		lag += deltaTimeMs;
		TotalTime += deltaTimeMs / 1000.0f;

		while (lag >= MS_PER_UPDATE)
		{
			Update(deltaTimeMs);
			lag -= MS_PER_UPDATE;
		}

		Draw();
	}
}

void Game::UpdateInternal()
{

}

void Game::Update(float deltaTime)
{
	UpdateInternal();

	for (auto gameObject : _gameObjects)
	{
		gameObject->Update(deltaTime);
	}
}

void Game::PrepareFrame()
{
	SetShadowRasterizerState();
	context->OMSetRenderTargets(0, nullptr, shadowDSV);
	context->ClearDepthStencilView(shadowDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (auto gameObject : _gameObjects)
	{
		gameObject->PrepareFrame();
	}
}

void Game::Draw()
{
	PrepareFrame();

	context->ClearState();
	context->RSSetViewports(1, &viewport);

	SetBaseRasterizerState();
	//context->RSSetState(rastState);

	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	context->OMSetRenderTargets(1, &mainRTV, mainDSV);
	context->ClearRenderTargetView(mainRTV, clearColor);
	context->ClearDepthStencilView(mainDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	FrameCount++;
	if (TotalTime > 1.0f) {
		float fps = FrameCount / TotalTime;

		TotalTime -= 1.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(Display->hWnd, text);

		FrameCount = 0;
	}

	for (auto gameObject : _gameObjects)
	{
		gameObject->Draw();
	}

	EndFrame();
}

void Game::EndFrame()
{
	context->OMSetRenderTargets(0, nullptr, nullptr);
	swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}

void Game::Exit()
{

}

#include "Game.h"
#include "GameObject.h"
#include "Components/LightComponent.h"

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
	SaveWICTextureToFile(context, gBuffer_.albedoBuffer_.Get(), GUID_ContainerFormatJpeg, L"SCREENSHOT_ALBEDO.JPG");
	SaveWICTextureToFile(context, gBuffer_.normalBuffer_.Get(), GUID_ContainerFormatJpeg, L"SCREENSHOT_NORMALS.JPG");
	SaveWICTextureToFile(context, gBuffer_.positionBuffer_.Get(), GUID_ContainerFormatJpeg, L"SCREENSHOT_POSITION.JPG");
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
	ShaderManager->InitShader(GetLightShader());

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
	context->RSSetViewports(1, &viewport);

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
	SetBaseRasterizerState();
	context->RSSetViewports(1, &viewport);

	context->OMSetDepthStencilState(defaultDepthState_.Get(), 0);

	//context->OMSetRenderTargets(1, &mainRTV, mainDSV);
	const auto rtvs = new ID3D11RenderTargetView * [3];
	rtvs[0] = gBuffer_.albedoRtv_.Get();
	rtvs[1] = gBuffer_.positionRtv_.Get();
	rtvs[2] = gBuffer_.normalRtv_.Get();
	context->OMSetRenderTargets(3, rtvs, mainDSV);


	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//context->ClearRenderTargetView(mainRTV, clearColor);
	context->ClearRenderTargetView(gBuffer_.albedoRtv_.Get(), clearColor);
	context->ClearRenderTargetView(gBuffer_.positionRtv_.Get(), clearColor);
	context->ClearRenderTargetView(gBuffer_.normalRtv_.Get(), clearColor);
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

	context->ClearState();
	SetLightRasterizerState();
	context->RSSetViewports(1, &viewport);

	context->OMSetDepthStencilState(quadDepthState_.Get(), 0);
	context->OMSetRenderTargets(1, &mainRTV, nullptr);

	ShaderManager->SetShader(GetLightShader());

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	LightComponent::Instance->Draw();

	context->PSSetShaderResources(0, 1, gBuffer_.albedoSrv_.GetAddressOf());
	context->PSSetShaderResources(1, 1, gBuffer_.positionSrv_.GetAddressOf());
	context->PSSetShaderResources(2, 1, gBuffer_.normalSrv_.GetAddressOf());

	context->Draw(4, 0);

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

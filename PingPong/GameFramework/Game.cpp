#include "Game.h"

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
}

void Game::CreateComponent(GameComponent* component)
{
	components_[componentsCount] = component;
	componentsCount++;
}

bool Game::Initialize()
{
	float totalTime = 0;
	unsigned int frameCount = 0;

	Display = new DisplayWin(name);

	if (!CreateBackBuffer())
		return false;

	Input = new InputManager();
	Input->Initialize();

	Shader = new ShaderManager();

	for (int i = 0; i < componentsCount; i++)
	{
		components_[i]->Initialize();
	}

	return true;
}

bool Game::CreateBackBuffer()
{
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = Display->ClientWidth;
	swapDesc.BufferDesc.Height = Display->ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = Display->hWnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags = 0;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&swapDesc,
		&swapChain,
		&device,
		nullptr,
		&context);

	if (FAILED(res))
	{
		MessageBoxA(0, "D3D11CreateDeviceAndSwapChain() failed", "Fatal Error", MB_OK);
		std::cout << GetLastError();
		return false;
	}

	ID3D11Texture2D* backTex;
	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);
	res = device->CreateRenderTargetView(backTex, nullptr, &renderView);
	backTex->Release();

	if (FAILED(res))
	{
		MessageBoxA(0, "CreateRenderTargetView() failed", "Fatal Error", MB_OK);
		return false;
	}

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = device->CreateRasterizerState(&rastDesc, &rastState);
	context->RSSetState(rastState);

	if (FAILED(res))
	{
		MessageBoxA(0, "CreateRasterizerState() failed", "Fatal Error", MB_OK);
		return false;
	}

	return true;
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

		std::cout << deltaTimeMs << std::endl;

		while (lag >= MS_PER_UPDATE)
		{
			Update(deltaTimeMs);
			lag -= MS_PER_UPDATE;
		}

		Draw();
	}
}

void Game::Update(float deltaTime)
{
	UpdateInternal();

	for (int i = 0; i < componentsCount; i++)
	{
		components_[i]->Update(deltaTime);
	}
}


void Game::UpdateInternal()
{

}

void Game::PrepareFrame()
{
	context->ClearState();
	context->RSSetState(rastState);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(Display->ClientWidth);
	viewport.Height = static_cast<float>(Display->ClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);

	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	context->OMSetRenderTargets(1, &renderView, nullptr);
	context->ClearRenderTargetView(renderView, color);
}

void Game::Draw()
{
	PrepareFrame();

	FrameCount++;
	if (TotalTime > 1.0f) {
		float fps = FrameCount / TotalTime;

		TotalTime -= 1.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(Display->hWnd, text);

		FrameCount = 0;
	}

	for (int i = 0; i < componentsCount; i++)
	{
		components_[i]->Draw();
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


LRESULT CALLBACK Game::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	LRESULT result = 0;
	switch (umessage)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		default:
		{
			result = Instance->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}

	return result;
}

LRESULT Game::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			Input->KeyUp((unsigned int)wparam);
			return 0;
		}
		default:
		{
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}

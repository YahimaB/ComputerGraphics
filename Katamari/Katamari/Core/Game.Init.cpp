#include "Game.h"

bool Game::Initialize()
{
	CreateDisplay();

	if (!CreateResources())
		return false;

	ShaderManager = new ::ShaderManager();
	InputDevice = new ::InputDevice(this);

	gBuffer = new GBuffer();
	gBuffer->Initialize();

	return true;
}

void Game::CreateDisplay()
{
	Display = new DisplayWin(name);

	viewport.Width = static_cast<float>(Display->ClientWidth);
	viewport.Height = static_cast<float>(Display->ClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;
}

bool Game::CreateResources()
{
	if (!CreateDeviceAndSwapChain())
		return false;

	if (!CreateMainRenderResources())
		return false;

	if (!CreateShadowRenderResources())
		return false;
	
	if (!CreateDepthStencilStates())
		return false;

	return true;
}

bool Game::CreateDeviceAndSwapChain()
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
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	HRESULT res = D3D11CreateDeviceAndSwapChain(
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
		return false;
	}

	return true;
}

bool Game::CreateMainRenderResources()
{
	HRESULT res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTex);
	if (FAILED(res))
	{
		MessageBoxA(0, "GetBuffer() for backBufferTex failed", "Fatal Error", MB_OK);
		return false;
	}

	res = device->CreateRenderTargetView(backBufferTex, nullptr, &mainRTV);
	if (FAILED(res))
	{
		MessageBoxA(0, "CreateRenderTargetView() for renderView failed", "Fatal Error", MB_OK);
		return false;
	}


	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = Display->ClientWidth;
	depthStencilDesc.Height = Display->ClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ID3D11Texture2D* depthTex;
	res = device->CreateTexture2D(&depthStencilDesc, nullptr, &depthTex);
	if (FAILED(res))
	{
		MessageBoxA(0, "CreateTexture2D() for depthTex failed", "Fatal Error", MB_OK);
		return false;
	}

	res = device->CreateDepthStencilView(depthTex, nullptr, &mainDSV);
	depthTex->Release();
	if (FAILED(res))
	{
		MessageBoxA(0, "CreateDepthStencilView() for mainDSV failed", "Fatal Error", MB_OK);
		return false;
	}

	return true;
}

bool Game::CreateShadowRenderResources()
{
	D3D11_TEXTURE2D_DESC depthDescription = {};
	depthDescription.Width = static_cast<float>(Display->ClientWidth);
	depthDescription.Height = static_cast<float>(Display->ClientWidth);
	depthDescription.ArraySize = 5;
	depthDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	depthDescription.Format = DXGI_FORMAT_R32_TYPELESS;
	depthDescription.MipLevels = 1;
	depthDescription.SampleDesc.Count = 1;
	depthDescription.SampleDesc.Quality = 0;
	depthDescription.Usage = D3D11_USAGE_DEFAULT;
	depthDescription.CPUAccessFlags = 0;
	depthDescription.MiscFlags = 0;

	ID3D11Texture2D* shadowTexArr;
	HRESULT res = device->CreateTexture2D(&depthDescription, nullptr, &shadowTexArr);
	if (FAILED(res))
	{
		MessageBoxA(0, "CreateTexture2D() for shadowTexArr failed", "Fatal Error", MB_OK);
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dViewDesc = {};
	dViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dViewDesc.Texture2DArray = {};
	dViewDesc.Texture2DArray.MipSlice = 0;
	dViewDesc.Texture2DArray.FirstArraySlice = 0;
	dViewDesc.Texture2DArray.ArraySize = 5;

	res = device->CreateDepthStencilView(shadowTexArr, &dViewDesc, &shadowDSV);
	if (FAILED(res))
	{
		MessageBoxA(0, "CreateDepthStencilView() for shadowDSV failed", "Fatal Error", MB_OK);
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray = {};
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = 5;

	res = device->CreateShaderResourceView(shadowTexArr, &srvDesc, &shadowSRV);
	shadowTexArr->Release();
	if (FAILED(res))
	{
		MessageBoxA(0, "CreateShaderResourceView() for shadowSRV failed", "Fatal Error", MB_OK);
		return false;
	}

	return true;
}

bool Game::SetBaseRasterizerState()
{
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = true;
	rastDesc.DepthClipEnable = true;

	HRESULT res = device->CreateRasterizerState(&rastDesc, &rastState);
	context->RSSetState(rastState);

	if (FAILED(res))
	{
		MessageBoxA(0, "CreateRasterizerState() failed", "Fatal Error", MB_OK);
		return false;
	}

	return true;
}

bool Game::SetShadowRasterizerState()
{
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_FRONT;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = true;
	rastDesc.DepthClipEnable = true;

	HRESULT res = device->CreateRasterizerState(&rastDesc, &rastState);
	context->RSSetState(rastState);

	if (FAILED(res))
	{
		MessageBoxA(0, "CreateShadowRasterizerState() failed", "Fatal Error", MB_OK);
		return false;
	}

	return true;
}

bool Game::SetLightRasterizerState()
{
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = true;
	rastDesc.DepthClipEnable = true;

	HRESULT res = device->CreateRasterizerState(&rastDesc, &rastState);
	context->RSSetState(rastState);

	if (FAILED(res))
	{
		MessageBoxA(0, "CreateLightRasterizerState() failed", "Fatal Error", MB_OK);
		return false;
	}

	return true;
}

bool Game::CreateDepthStencilStates()
{
	D3D11_DEPTH_STENCIL_DESC defaultDepthDesc = {};
	defaultDepthDesc.DepthEnable = true;
	defaultDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	defaultDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;

	HRESULT res = device->CreateDepthStencilState(&defaultDepthDesc, &defaultDepthState);
	if (FAILED(res))
	{
		MessageBoxA(0, "CreateDepthStencilStates() for default failed", "Fatal Error", MB_OK);
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC quadDepthDesc = {};
	defaultDepthDesc.DepthEnable = true;
	defaultDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	defaultDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;

	HRESULT res2 = device->CreateDepthStencilState(&quadDepthDesc, &quadDepthState);

	if (FAILED(res2))
	{
		MessageBoxA(0, "CreateDepthStencilStates() for quad failed", "Fatal Error", MB_OK);
		return false;
	}

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE; //D3D11_BLEND_SRC_COLOR;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; //D3D11_BLEND_BLEND_FACTOR;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
	blendDesc.AlphaToCoverageEnable = false;

	HRESULT res3 = device->CreateBlendState(&blendDesc, &blendState);

	if (FAILED(res3))
	{
		MessageBoxA(0, "CreateDepthStencilStates() for blendState failed", "Fatal Error", MB_OK);
		return false;
	}

	return true;
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
		if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
		return 0;
	}

	case WM_INPUT:
	{
		UINT dwSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == nullptr) {
			return 0;
		}

		if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			//printf(" Kbd: make=%04i Flags:%04i Reserved:%04i ExtraInformation:%08i, msg=%04i VK=%i \n",
			//	raw->data.keyboard.MakeCode,
			//	raw->data.keyboard.Flags,
			//	raw->data.keyboard.Reserved,
			//	raw->data.keyboard.ExtraInformation,
			//	raw->data.keyboard.Message,
			//	raw->data.keyboard.VKey);

			InputDevice->OnKeyDown({
				raw->data.keyboard.MakeCode,
				raw->data.keyboard.Flags,
				raw->data.keyboard.VKey,
				raw->data.keyboard.Message
				});
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			//printf(" Mouse: X=%04d Y:%04d \n", raw->data.mouse.lLastX, raw->data.mouse.lLastY);
			InputDevice->OnMouseMove({
				raw->data.mouse.usFlags,
				raw->data.mouse.usButtonFlags,
				static_cast<int>(raw->data.mouse.ulExtraInformation),
				static_cast<int>(raw->data.mouse.ulRawButtons),
				static_cast<short>(raw->data.mouse.usButtonData),
				raw->data.mouse.lLastX,
				raw->data.mouse.lLastY
				});
		}

		delete[] lpb;
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}

	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}
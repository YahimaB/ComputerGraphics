#pragma once

/*----------------------------------------------------------------------------
	Windows includes.
----------------------------------------------------------------------------*/

#define NOMINMAX
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>

/*----------------------------------------------------------------------------
	DirectX includes.
----------------------------------------------------------------------------*/

#include <d3d.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

/*----------------------------------------------------------------------------
	Local DirectXTK includes.
----------------------------------------------------------------------------*/

#include "DirectXTK/SimpleMath.h"
#include "DirectXTK/PlatformHelpers.h"

/*----------------------------------------------------------------------------
	Other includes.
----------------------------------------------------------------------------*/

#include <chrono>
#include <iostream>
#include <vector>


class Game;

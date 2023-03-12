#pragma once

#include "../RendererComponent.h"

class SphereComponent : public RendererComponent
{
public:
	SphereComponent(float radius, int sliceCount = 32, int stackCount = 32);

protected:
	LPCWSTR GetShaderName() override { return L"./Shaders/My3DShader.hlsl"; }
};


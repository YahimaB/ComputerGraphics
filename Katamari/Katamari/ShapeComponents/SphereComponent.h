#pragma once

#include "../Core/Components/RendererComponent.h"
#include "../Core/Game.h"

class SphereComponent : public RendererComponent
{
public:
	SphereComponent(float radius, int sliceCount = 32, int stackCount = 32);

	void Update(float deltaTime) override;

protected:
	LPCWSTR GetShaderName() override { return L"./Shaders/My3DShader.hlsl"; }
};


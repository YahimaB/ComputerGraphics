#pragma once

#include "../Core/Components/ShapeComponent.h"
#include "../Core/Game.h"

class SphereComponent : public ShapeComponent
{
public:
	Quaternion Rotation = Quaternion::Identity;
	Vector3 Position = Vector3::Zero;

public:
	SphereComponent(float radius, int sliceCount = 32, int stackCount = 32);

	void Update(float deltaTime) override;

protected:
	LPCWSTR GetShaderName() override { return L"./Shaders/My3DShader.hlsl"; }
};


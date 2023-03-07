#pragma once

#include "../Core/Components/ShapeComponent.h"
#include "../Core/Game.h"

class SphereComponent : public ShapeComponent
{
public:
	DirectX::SimpleMath::Quaternion Rotation;
	DirectX::SimpleMath::Vector3 Position;

public:
	SphereComponent(float radius, int sliceCount, int stackCount, DirectX::SimpleMath::Vector4 col1, DirectX::SimpleMath::Vector4 col2);

	void Update(float deltaTime) override;

protected:
	LPCWSTR GetShaderName() override { return L"./Shaders/My3DShader.hlsl"; }
};


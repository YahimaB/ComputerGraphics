#pragma once

#include "../Game.h"
#include "../GameComponent.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

struct Light
{
	Vector4 Direction;
	Vector4 Color;
};

struct LightingData
{
	Light Lights;
	Vector3 ViewVector;
	float Intensity;
};

struct CascadeData
{
	Matrix ViewProj[5];
	Vector4 Distance;
};

class LightComponent : public GameComponent
{
public:
	static LightComponent* Instance;

public:
	LightComponent();

	void Initialize() override;
	void Update(float deltaTime) override;
	void PrepareFrame() override;
	void Draw() override;
	void DestroyResources() override;

protected:
	Game* Game = nullptr;
	Camera* Camera = nullptr;

	ID3D11Buffer* constLightsBuffer = nullptr;
	ID3D11Buffer* constCascadeBuffer = nullptr;

	ID3D11SamplerState* depthSamplerState = nullptr;

	const static int CascadeCount = 4;
	float cascadeOffsets[CascadeCount] = {50.0f, 25.0f, 10.0f, 2.0f};
	float shadowCascadeLevels[CascadeCount] = {};

public:
	Matrix GetLightProjViewMatrix(float nearPlane, float farPlane);
	Vector4 GetShadowCascadeDistances() const;
	std::vector<Matrix> GetLightProjViewMatrices();
};


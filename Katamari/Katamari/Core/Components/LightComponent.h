#pragma once

#include "../Game.h"
#include "../GameComponent.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

struct Light
{
	Vector4 Position = Vector4::Zero;
	Vector4 Direction;
	Vector4 Color;
	float SpotAngle = 1.0f;              // 4 bytes
	float ConstantAttenuation = 0.0f;    // 4 bytes
	float LinearAttenuation = 0.1f;      // 4 bytes
	float QuadraticAttenuation = 0.0f;

	int LightType = 0;              // 4 bytes
	bool Enabled = false;                // 4 bytes
	int Padding[2];
};

struct LightingData
{
	Light Lights;
	Matrix ViewMatrix;
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

	int Type = 0;
	Vector3 Color = Vector3::Zero;

public:
	LightComponent(int type = 0, Vector3 color = Vector3::One);

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

	std::string GetUniqueId() override { return "light"; }

public:
	Matrix GetLightProjViewMatrix(float nearPlane, float farPlane);
	Vector4 GetShadowCascadeDistances() const;
	std::vector<Matrix> GetLightProjViewMatrices();
};


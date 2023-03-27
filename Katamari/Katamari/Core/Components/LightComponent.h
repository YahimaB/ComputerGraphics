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

struct LightProperties
{
	Light Lights;
	Vector3 ViewVector;
	float Intensity;
	Matrix T;
};

class LightComponent : public GameComponent
{
public:
	static LightComponent* Instance;

public:
	LightComponent();

	void Initialize() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void DestroyResources() override;

protected:
	Game* Game = nullptr;
	Camera* Camera = nullptr;

	ID3D11Buffer* constBuffer = nullptr;

public:
	std::vector<float> shadowCascadeLevels_{};
	Matrix GetLightSpaceMatrix(float nearPlane, float farPlane);
	Vector4 GetShadowCascadeDistances() const;
	std::vector<Matrix> GetLightSpaceMatrices();
};


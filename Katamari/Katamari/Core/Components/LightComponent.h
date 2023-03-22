#pragma once

#include "../Game.h"
#include "../GameComponent.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

struct CBDataPerScene
{
	Vector4 lightPos;
	Vector4 lightColorAmbStr;
	Vector4 viewDirSpecStr;
};

class LightComponent : public GameComponent
{
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
};


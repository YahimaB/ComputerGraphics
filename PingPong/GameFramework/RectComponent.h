#pragma once

#include "GameComponent.h"

class RectComponent : public GameComponent
{
public:
	RectComponent();
	~RectComponent();

	void Initialize() override;
	void Draw() override;
	void Update() override;
	void DestroyResources() override;

protected:
	LPCWSTR ShaderName = L"MyVeryFirstShader.hlsl";

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};


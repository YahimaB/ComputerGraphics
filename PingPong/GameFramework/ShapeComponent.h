#pragma once

#include "GameComponent.h"

#define float4 DirectX::XMFLOAT4

struct Vertex {
	float4 pos;
	float4 col = float4(1.0f, 1.0f, 1.0f, 1.0f);
};

class ShapeComponent : public GameComponent
{
public:
	ShapeComponent();

	void Initialize() override;
	void Draw() override;

protected:
	LPCWSTR ShaderName = L"MyVeryFirstShader.hlsl";

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	int pointsCount;
	int indicesCount;
	Vertex points[30];
	int indices[100];
};


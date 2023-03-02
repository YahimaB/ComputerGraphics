#pragma once

#include "GameComponent.h"

struct Vertex {
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 col;
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
	DirectX::XMFLOAT4 points[30];
	int indices[30];
};


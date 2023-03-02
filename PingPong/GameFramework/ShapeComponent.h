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
	~ShapeComponent();

	void Initialize() override;
	void Draw() override;
	void Update() override;
	void DestroyResources() override;

protected:
	LPCWSTR ShaderName = L"MyVeryFirstShader.hlsl";

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};


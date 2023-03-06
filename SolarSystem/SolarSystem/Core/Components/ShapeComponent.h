#pragma once

#include "../Game.h"
#include "../GameComponent.h"

#define float4 DirectX::XMFLOAT4

struct ConstBuf {
	float4 Offset;
};

struct Vertex {
	float4 pos;
	float4 col = float4(1.0f, 1.0f, 1.0f, 1.0f);
};

struct CustomRect {
	float left;
	float bot;
	float top;
	float right;
};

class ShapeComponent : public GameComponent
{
public:
	ShapeComponent();

	void Initialize() override;
	void Draw() override;

	virtual CustomRect* GetRect();
	bool Intersects(const CustomRect& rect1, const CustomRect& rect2);

protected:
	LPCWSTR ShaderName = L"MyVeryFirstShader.hlsl";

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* constBuffer;


	int pointsCount;
	int indicesCount;
	Vertex points[30];
	int indices[100];
};


#pragma once

#include "../Game.h"
#include "../GameComponent.h"
#include "../DirectXTK/SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;


struct ConstBuff {
	Matrix World_View_Projection;
};

struct Vertex {
	Vector4 pos = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	Vector4 col = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
};

class ShapeComponent : public GameComponent
{
public:
	ShapeComponent() {};
	~ShapeComponent() {};

	void Initialize() override;
	void Draw() override;

protected:

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* constBuffer;

	std::vector<Vertex> points;
	std::vector<int> indices;

protected:

	virtual LPCWSTR GetShaderName() { return L"./Shaders/MyVeryFirstShader.hlsl"; }
	virtual D3D_PRIMITIVE_TOPOLOGY GetTopology() { return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; }
};


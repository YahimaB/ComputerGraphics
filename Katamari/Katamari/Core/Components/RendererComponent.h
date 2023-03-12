#pragma once

#include "../Game.h"
#include "../GameComponent.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

struct ConstBuff {
	Matrix World_View_Projection;
};

struct Vertex {
	Vector4 pos = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	Vector4 col = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
};

class RendererComponent : public GameComponent
{
public:
	RendererComponent();

	void Initialize() override;
	void Draw() override;
	void DestroyResources() override;

protected:
	Game* Game = nullptr;
	Camera* Camera = nullptr;

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11Buffer* constBuffer = nullptr;

	std::vector<Vertex> points;
	std::vector<int> indices;

protected:
	std::string GetUniqueId() override { return "rc"; }

	virtual LPCWSTR GetShaderName() { return L"./Shaders/MyVeryFirstShader.hlsl"; }
	virtual D3D_PRIMITIVE_TOPOLOGY GetTopology() { return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; }
};


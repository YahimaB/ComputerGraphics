#pragma once

#include "../Game.h"
#include "../GameComponent.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

struct ObjectData {
	Matrix World;
	Matrix WorldView;
	Matrix World_View_Projection;
	Matrix invTrWorld;
};

struct Vertex {
	Vector4 pos = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	Vector4 tex;
	Vector4 normal;
};

class RendererComponent : public GameComponent
{
public:
	RendererComponent();

	void Initialize() override;
	void Update(float deltaTime) override;
	void PrepareFrame() override;
	void Draw() override;
	void DestroyResources() override;

protected:
	Game* Game = nullptr;
	Camera* Camera = nullptr;

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11Buffer* constObjectBuffer = nullptr;

	ID3D11SamplerState* samplerState = nullptr;

	std::vector<Vertex> points;
	std::vector<int> indices;

	UINT strides[1] = { sizeof(Vertex) };
	UINT offsets[1] = { 0 };

protected:
	std::string GetUniqueId() override { return "rc"; }

	virtual std::string GetTextureName() { return "./Textures/default.dds"; }

	virtual FShaderData GetBaseShader() { return { "./Shaders/BufferShader.hlsl", ST_Vertex | ST_Pixel }; }
	virtual FShaderData GetShadowShader() { return { "./Shaders/Csm.hlsl", ST_Vertex | ST_Geometry }; }

	virtual D3D_PRIMITIVE_TOPOLOGY GetTopology() { return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; }
};


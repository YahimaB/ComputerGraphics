#pragma once

#include "../Core/Components/ShapeComponent.h"

class GridComponent : public ShapeComponent
{
public:
	GridComponent(float cellSize = 1.0f, int lineCount = 100);

	void Update(float deltaTime) override;

protected:
	LPCWSTR GetShaderName() override { return L"./Shaders/My3DShader.hlsl"; }
	D3D_PRIMITIVE_TOPOLOGY GetTopology() override { return D3D_PRIMITIVE_TOPOLOGY_LINELIST; }

private:
	Vector4 GetColor(int index);
};


#pragma once

#include "../RendererComponent.h"

class GridComponent : public RendererComponent
{
public:
	GridComponent(float cellSize = 1.0f, int lineCount = 100);

protected:
	D3D_PRIMITIVE_TOPOLOGY GetTopology() override { return D3D_PRIMITIVE_TOPOLOGY_LINELIST; }

private:
	Vector4 GetColor(int index);
};


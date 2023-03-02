#include "SuperShapeComponent.h"

SuperShapeComponent::SuperShapeComponent()
{
	pointsCount = 8;
	points[0] = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	points[1] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	points[2] = DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f);
	points[3] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	points[4] = DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f);
	points[5] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	points[6] = DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f);
	points[7] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	indicesCount = 6;
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 0;
	indices[5] = 3;
}

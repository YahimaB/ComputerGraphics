#include "PlatformShapeComponent.h"

PlatformShapeComponent::PlatformShapeComponent(float posX) : ShapeComponent()
{
	float leftPoint = -0.05f + posX;
	float rightPoint = 0.05f + posX;

	pointsCount = 4;
	points[0].pos = float4(leftPoint, 0.25f, 0.25f, 1.0f);
	points[1].pos = float4(rightPoint, -0.25f, 0.25f, 1.0f);
	points[2].pos = float4(leftPoint, -0.25f, 0.25f, 1.0f);
	points[3].pos = float4(rightPoint, 0.25f, 0.25f, 1.0f);

	indicesCount = 6;
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 0;
	indices[5] = 3;
}
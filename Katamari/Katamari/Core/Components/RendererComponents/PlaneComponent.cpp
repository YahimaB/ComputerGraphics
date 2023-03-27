#include "PlaneComponent.h"

PlaneComponent::PlaneComponent(float size)
{
	float pos = size;

	points.push_back({ Vector4(size, 0.0f, size,1.0f),	Vector4(8.0f, 8.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) });
	points.push_back({ Vector4(-size, 0.0f, -size, 1.0f),Vector4(0.0f, 0.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) });
	points.push_back({ Vector4(-size, 0.0f, size, 1.0f), Vector4(0.0f, 8.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) });
	points.push_back({ Vector4(size, 0.0f, -size, 1.0f), Vector4(8.0f, 0.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) });

	indices = { 0,1,2, 1,0,3 };
}

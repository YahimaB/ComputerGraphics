#include "CircleShapeComponent.h"

CircleShapeComponent::CircleShapeComponent(float radius)
{
	this->radius = radius;

	pointsCount = 25;
	indicesCount = (pointsCount - 1) * 3;

	InitPoints();
	InitIndeces();
}

void CircleShapeComponent::InitPoints() {
	points[0].pos = float4(0.0f, 0.0f, 0.0f, 1.0f);

	double PI = 2 * acos(0.0);
	double step = PI * 2 / (pointsCount - 1);
	double a = 0.0f;
	int i = 1;

	while (a < PI * 2) {
		float x, y;
		x = cos(a) * radius;
		y = sin(a) * radius;

		points[i].pos = float4(x, y, 0.0f, 1.0f);

		i++;
		a += step;
	}
}

void CircleShapeComponent::InitIndeces() {

	int sl = 1;

	for (int i = 0; i < indicesCount; i += 3) {
		if ((i + 3) < indicesCount) {
			indices[i] = 0;
			indices[i + 1] = sl;
			indices[i + 2] = ++sl;
		}
		else {
			indices[i] = 0;
			indices[i + 1] = indicesCount / 3;
			indices[i + 2] = 1;
		}
	}

};
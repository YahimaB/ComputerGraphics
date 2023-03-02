#pragma once

#include "ShapeComponent.h"

class CircleShapeComponent : public ShapeComponent
{
public:
	CircleShapeComponent(float radius);

private:
	float radius = 0.05f;

	void InitPoints();
	void InitIndeces();
};


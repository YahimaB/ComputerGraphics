#pragma once
#include "ShapeComponent.h"

class PlatformShapeComponent : public ShapeComponent
{
public:
	PlatformShapeComponent(float posX);

	void Update(float deltaTime) override;
	CustomRect* GetRect() override;

private:
	float offsetY = 0.0f;
};

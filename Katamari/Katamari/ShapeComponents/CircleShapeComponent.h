#pragma once

#include "../Core/Components/ShapeComponent.h"
#include "../Core/Game.h"
#include "PlatformShapeComponent.h"

class CircleShapeComponent : public ShapeComponent
{
public:
	CircleShapeComponent(float radius, PlatformShapeComponent* platforms[]);

	void Update(float deltaTime) override;
	CustomRect* GetRect() override;

private:
	float radius = 0.05f;

	void InitPoints();
	void InitIndeces();

	void Restart();

	float startSpeed;
	float currentSpeed;

	float offsetX = 0.0f;
	float offsetY = 0.0f;

	float4 direction = float4(1.0f, 0.0f, 0.0f, 0.0f);

	PlatformShapeComponent* Platforms[2];
	CustomRect Borders[2];
	CustomRect LoseZones[2];
	int scores[2];

	int randNum(int min, int max) { return rand() % (max - min + 1) + min; }
};


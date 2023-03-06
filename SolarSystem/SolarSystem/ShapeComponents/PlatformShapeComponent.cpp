#include "PlatformShapeComponent.h"
#include "../Core/InputManager.h"

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

void PlatformShapeComponent::Update(float deltaTime)
{
	auto distance = 0.001f * deltaTime;

	if (InputManager::Instance->IsKeyDown(Keys::Down))
	{
		if (offsetY + points[1].pos.y < -1.0f)
			return;

		offsetY -= distance;
	}

	if (InputManager::Instance->IsKeyDown(Keys::Up))
	{
		if (offsetY + points[0].pos.y > 1.0f)
			return;

		offsetY += distance;
	}

	ConstBuf constBuf;
	constBuf.Offset = float4(0.0f, offsetY, 0.0f, 0.0f);
	game->context->UpdateSubresource(constBuffer, 0, NULL, &constBuf, 0, 0);
}

CustomRect* PlatformShapeComponent::GetRect()
{
	CustomRect rect = {};

	rect.left = points[2].pos.x;
	rect.bot = points[2].pos.y + offsetY;

	rect.right = points[3].pos.x;
	rect.top = points[3].pos.y + offsetY;

	return &rect;
}


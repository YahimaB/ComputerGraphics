#include "DelimiterShapeComponent.h"

DelimiterShapeComponent::DelimiterShapeComponent()
{
	pointsCount = 4;
	points[0].pos = float4(-0.015f, 1.0f, 0.25f, 1.0f);
	points[1].pos = float4(0.015f, 1.0f, 0.25f, 1.0f);
	points[2].pos = float4(-0.015f, 0.92f, 0.25f, 1.0f);
	points[3].pos = float4(0.015f, 0.92f, 0.25f, 1.0f);

	indicesCount = 6;
	indices[0] = 0;
	indices[1] = 3;
	indices[2] = 1;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 2;
}

void DelimiterShapeComponent::Draw()
{
	for (float i = 0.0f; i < 2.0f; i += 0.215f) {
		ConstBuf constBuf;
		constBuf.Offset = float4(0.0f, i * -1.0f, 0.0f, 0.0f);
		game->context->UpdateSubresource(constBuffer, 0, NULL, &constBuf, 0, 0);


		ShapeComponent::Draw();
	}

	return;
}

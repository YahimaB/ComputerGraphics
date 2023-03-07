#include "GridComponent.h"

GridComponent::GridComponent(float cellSize, int lineCount)
{
	const int halfCount = lineCount / 2;
	const int halfSize = cellSize * halfCount;

	int indiciesCount = 0;
	for (int i = -halfSize; i < halfSize; i+= cellSize)
	{
		points.push_back(Vertex{ Vector4(i, 0.0f, -halfSize, 1.0f), GetColor(i)});
		points.push_back(Vertex{ Vector4(i, 0.0f, halfSize, 1.0f), GetColor(i)});

		indices.push_back(indiciesCount++);
		indices.push_back(indiciesCount++);

		points.push_back(Vertex({ Vector4(-halfSize, 0.0f, i, 1.0f), GetColor(i) }));
		points.push_back(Vertex({ Vector4(halfSize, 0.0f, i, 1.0f), GetColor(i) }));

		indices.push_back(indiciesCount++);
		indices.push_back(indiciesCount++);
	}
}

void GridComponent::Update(float deltaTime)
{
	Matrix worldViewProj = game->Camera->GetMatrix();
	worldViewProj = worldViewProj.Transpose();
	game->context->UpdateSubresource(constBuffer, 0, nullptr, &worldViewProj, 0, 0);
}

Vector4 GridComponent::GetColor(int index)
{
	auto visibility = index % 5 == 0 ? 0.3f : 0.1f;

	return Vector4(Vector3::One * visibility);
}

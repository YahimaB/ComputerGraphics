#include "SphereComponent.h"

SphereComponent::SphereComponent(float radius, int sliceCount, int stackCount)
{
	Vertex topVertex({ Vector4(0.0f, radius, 0.0f, 1.0f) });
	Vertex bottomVertex({ Vector4(0.0f, -radius, 0.0f, 1.0f) });

	points.push_back(topVertex);

	float phiStep = XM_PI / stackCount;
	float thetaStep = 2.0f * XM_PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			Vertex v;

			// spherical to cartesian
			v.pos.x = radius * sinf(phi) * cosf(theta);
			v.pos.y = radius * cosf(phi);
			v.pos.z = radius * sinf(phi) * sinf(theta);
			
			v.col = v.pos;

			points.push_back(v);
		}
	}

	points.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			indices.push_back(baseIndex + i * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = points.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}
}

void SphereComponent::Update(float deltaTime)
{
	Rotation.Normalize();
	Matrix world = Matrix::CreateScale(Scale) * Matrix::CreateFromQuaternion(Rotation) * Matrix::CreateTranslation(Position);
	Matrix worldViewProj = world * game->Camera->GetViewProjectionMatrix();

	auto buffMatrix = worldViewProj.Transpose();
	game->context->UpdateSubresource(constBuffer, 0, nullptr, &buffMatrix, 0, 0);
}
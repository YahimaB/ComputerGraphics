#pragma once

#include "../MinimalCore.h"
#include "../GameComponent.h"

using namespace DirectX;
using namespace SimpleMath;

class Camera : public GameComponent
{
public:
	static Camera* Current;

	enum class ProjectionType
	{
		Perspective = 0,
		Orthographic = 1
	};

	struct ClippingPlanes
	{
		float Near = 1.0f;
		float Far = 1000.0f;
	};

public:
	ProjectionType Projection = ProjectionType::Perspective;
	ClippingPlanes ClipPlanes = {};
	float AspectRatio = 1.0f;

	float FOV = XM_PI / 4.0f;
	float OrthoSize = 40.0f;

public:
	Camera();

	void Update(float deltaTime) override;

	DirectX::SimpleMath::Matrix GetViewMatrix() const;
	DirectX::SimpleMath::Matrix GetProjectionMatrix() const;

	DirectX::SimpleMath::Matrix GetViewProjectionMatrix() const;

	std::vector<Vector4> GetFrustumCornersWorldSpace(const Matrix& view, const Matrix& proj);

protected:
	DirectX::SimpleMath::Matrix ViewProjectionMatrix = Matrix::Identity;

};


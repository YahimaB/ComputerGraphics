#pragma once
#include "../DirectXTK/SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

class Camera
{
public:
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

	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Target;
	DirectX::SimpleMath::Vector3 Up;

public:
	ProjectionType Projection = ProjectionType::Perspective;
	ClippingPlanes ClipPlanes = {};
	float AspectRatio = 1.0f;

	float FOV = XM_PI / 4.0f;
	float OrthoSize = 40.0f;

public:
	Camera();

	void UpdateMatrix();

	DirectX::SimpleMath::Matrix GetViewMatrix() const;
	DirectX::SimpleMath::Matrix GetProjectionMatrix() const;

	DirectX::SimpleMath::Matrix GetMatrix() const;

protected:
	DirectX::SimpleMath::Matrix viewProj = Matrix::Identity;

};


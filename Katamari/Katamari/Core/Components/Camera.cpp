#include "Camera.h"
#include "Transform.h"

Camera* Camera::Current = 0;

Camera::Camera()
{
	if (Current == nullptr)
	{
		Current = this;
	}

	//Camera->AspectRatio = static_cast<float>(Display->ClientWidth) / static_cast<float>(Display->ClientHeight);
}

void Camera::Update(float deltaTime)
{
	ViewProjectionMatrix = GetViewMatrix() * GetProjectionMatrix();
}

Matrix Camera::GetViewProjectionMatrix() const
{
	return ViewProjectionMatrix;
}

Matrix Camera::GetViewMatrix() const
{
	auto model = Transform->GetModel();

	auto up = model.Up();
	auto forward = model.Forward();
	auto position = model.Translation();
	auto target = position + forward;

	return Matrix::CreateLookAt(position, target, up);
}

Matrix Camera::GetProjectionMatrix() const
{
	if (Projection == ProjectionType::Perspective)
	{
		return Matrix::CreatePerspectiveFieldOfView(FOV, AspectRatio, ClipPlanes.Near, ClipPlanes.Far);
	}
	else 
	{
		return Matrix::CreateOrthographic(OrthoSize * AspectRatio, OrthoSize, ClipPlanes.Near, ClipPlanes.Far);
	}
}


std::vector<Vector4> Camera::GetFrustumCornersWorldSpace(const Matrix& view, const Matrix& proj)
{
	const auto viewProj = view * proj;
	const auto inv = viewProj.Invert();

	std::vector<Vector4> frustumCorners;
	frustumCorners.reserve(8);
	for (unsigned int x = 0; x < 2; ++x)
	{
		for (unsigned int y = 0; y < 2; ++y)
		{
			for (unsigned int z = 0; z < 2; ++z)
			{
				const Vector4 pt = 
					Vector4::Transform(Vector4(
						2.0f * x - 1.0f,
						2.0f * y - 1.0f,
						z,
						1.0f), inv);
				frustumCorners.push_back(pt / pt.w);
			}
		}
	}

	return frustumCorners;
}
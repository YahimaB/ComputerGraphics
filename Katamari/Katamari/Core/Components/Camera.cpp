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
	Vector3 forward = Vector3::Transform(Vector3::Forward, Transform->Rotation);

	auto position = Transform->Position;
	auto target = position + forward;
	auto up = Vector3::Transform(Vector3::Up, Transform->Rotation);

	return Matrix::CreateLookAt(Transform->Position, target, up);
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
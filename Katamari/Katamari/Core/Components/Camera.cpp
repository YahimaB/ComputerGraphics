#include "Camera.h"

Camera* Camera::Current = 0;

Camera::Camera()
{
	Position = Vector3::Backward * 10.0f;
	Target = Vector3::Up;
	Up = Vector3::Up;

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
	return Matrix::CreateLookAt(Position, Target, Up);
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
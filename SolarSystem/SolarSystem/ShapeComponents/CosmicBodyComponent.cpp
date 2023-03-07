#include "CosmicBodyComponent.h"

CosmicBodyComponent::CosmicBodyComponent(float radius, float orbitRadius, float orbitSpeed, Vector3* orbitPos)
	: SphereComponent(radius)
{
	this->orbitRadius = orbitRadius;
	this->orbitSpeed = orbitSpeed;

	if (orbitPos != nullptr)
	{
		orbitPosition = orbitPos;
	}

	Vector3 forward = XMVector3Orthogonal(orbitAxis) * -1.0f;

	childPosition = forward * orbitRadius;
}

void CosmicBodyComponent::Update(float deltaTime)
{
	childPosition = Vector3::Transform(childPosition, Matrix::CreateFromAxisAngle(orbitAxis, orbitSpeed * deltaTime * timeScale));
	Position = *orbitPosition + childPosition;

	Rotation *= Quaternion::CreateFromAxisAngle(spinAxis, spinSpeed * deltaTime * timeScale);

	SphereComponent::Update(deltaTime);
}
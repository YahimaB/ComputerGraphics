#pragma once

#include "SphereComponent.h"


class CosmicBodyComponent : public SphereComponent
{
public:
    CosmicBodyComponent(float radius, float orbitRadius = 0.0f, float orbitSpeed = 0.0f, Vector3* orbitPos = nullptr);

    void Update(float deltaTime) override;

protected:
    Vector3 spinAxis = Vector3::Up;
    float spinSpeed = 1.0f;

    Vector3 orbitAxis = Vector3::Up;
    float orbitSpeed;
    float orbitRadius;
    
    Vector3* orbitPosition = new Vector3(0.0f);
    Vector3 childPosition;

private:
    float timeScale = 0.001f;
};


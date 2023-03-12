#pragma once

//#include "../Core/MinimalCore.h"
#include "../Core/Components/RendererComponents/SphereComponent.h"

class KatamariBall : public SphereComponent
{
public:
    //KatamariBallOutline* outline;
    float rotationDrag;
    float rotationMaxSpeed;
    float moveMaxSpeed;
    float moveDrag;
    DirectX::SimpleMath::Quaternion savedRot;
    DirectX::SimpleMath::Vector3 velocity;
    //DirectX::BoundingSphere collision;
    float gameSize;

public:
    KatamariBall();
    ~KatamariBall();
    //void Initialize() override;
    //void Draw() override;
    void Update(float deltaTime) override;
    //void Reload() override;
    //void DestroyResources() override;
    void SetDirection(DirectX::SimpleMath::Vector3 dir);

    Vector3 GetForward() const;
    Vector3 GetUp() const;

protected:
    void UpdateSize(float absorbedSize);
};


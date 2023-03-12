#include "KatamariItem.h"

KatamariItem::KatamariItem() : SphereComponent(1.0f)
{
}

void KatamariItem::Initialize()
{
    //outline->Initialize();
    SphereComponent::Initialize();

    Collision = BoundingSphere(Transform->Position, 1.0f);
}

void KatamariItem::Update(float deltaTime)
{
    Collision.Center = Transform->Position;
    SphereComponent::Update(deltaTime);
}

#include "KatamariItem.h"

KatamariItem::KatamariItem(float radius)
{
    _radius = radius;
}

void KatamariItem::Initialize()
{
    //outline->Initialize();
    GameComponent::Initialize();

    Collision = BoundingSphere(Transform->Position, _radius);
}

void KatamariItem::Update(float deltaTime)
{
    Collision.Center = Transform->GetModel().Translation();
    GameComponent::Update(deltaTime);
}

float KatamariItem::GetRadius()
{
    return _radius;
}

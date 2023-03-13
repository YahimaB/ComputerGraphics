#include "KatamariItem.h"
#include "KatamariBall.h"
#include "../Core/Components/Transform.h"

#include "../Core/Game.h"
#include "../Core/GameObject.h"

using namespace DirectX;
using namespace SimpleMath;

KatamariBall::KatamariBall(float radius) : SphereComponent(radius)
{
	_radius = radius;
}

void KatamariBall::Initialize()
{
	SphereComponent::Initialize();

	collision = BoundingSphere(Transform->Position, _radius);
}

void KatamariBall::Update(float deltaTime)
{
	collision.Center = Transform->GetModel().Translation();

	auto items = Game->FindGameObjects("Item");
	for (auto item : items)
	{
		KatamariItem* kItem = (KatamariItem*)item->GetComponent("KatamariItem");

		if (!kItem->PickedUp && collision.Intersects(kItem->Collision))
		{
			std::cout << "Pick" << std::endl;
			kItem->PickedUp = true;
			kItem->Transform->SetParent(Transform);

			Quaternion inv;
			auto worldRotation = Quaternion::CreateFromRotationMatrix(Transform->GetModel());
			worldRotation.Inverse(inv);

			auto localPos = kItem->Transform->Position - Transform->GetModel().Translation();
			kItem->Transform->Position = Vector3::Transform(localPos, inv);
			worldRotation.Inverse(kItem->Transform->Rotation);

			float itemRadius = kItem->GetRadius();
			float newRadius = sqrtf(_radius * _radius + itemRadius * itemRadius);

			_radius = newRadius;
			collision.Radius = _radius;
			Transform->Position.y = _radius;
		}
	}

	auto parentPos = Transform->GetParent()->Position;
	auto posDiff = parentPos - oldPos;
	oldPos = parentPos;

	if (posDiff.Length() > 0.0f)
	{
		float radius = 1.0f;
		float angle = posDiff.Length() / radius;

		Quaternion inv;
		Transform->GetParent()->Rotation.Inverse(inv);
		posDiff = Vector3::Transform(posDiff, inv);

		Quaternion q = Quaternion::CreateFromAxisAngle(posDiff.Cross(Vector3::Up), -angle);
		Transform->Rotation *= q;
	}

	SphereComponent::Update(deltaTime);
}

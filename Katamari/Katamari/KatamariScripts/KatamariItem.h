#pragma once

#include "../Core/Components/RendererComponents/SphereComponent.h"

class KatamariItem : public SphereComponent
{
public:
	bool PickedUp = false;

public:
	KatamariItem();
	void Initialize() override;
	void Update(float deltaTime) override;

	BoundingSphere Collision{};

protected:
	std::string GetUniqueId() override { return "KatamariItem"; }
};


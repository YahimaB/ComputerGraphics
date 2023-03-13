#pragma once

#include "../Core/GameComponent.h"

class KatamariItem : public GameComponent
{
public:
	BoundingSphere Collision{};

	bool PickedUp = false;

public:
	KatamariItem(float radius);
	void Initialize() override;
	void Update(float deltaTime) override;

	float GetRadius();

protected:
	std::string GetUniqueId() override { return "KatamariItem"; }

private:
	float _radius;
};


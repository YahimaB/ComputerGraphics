#pragma once

#include "../RendererComponent.h"

class SphereComponent : public RendererComponent
{
public:
	SphereComponent(float radius, int sliceCount = 32, int stackCount = 32);
};


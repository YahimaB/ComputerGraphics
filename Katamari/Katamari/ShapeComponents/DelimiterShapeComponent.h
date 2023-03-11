#pragma once

#include "../Core/Components/ShapeComponent.h"
#include "../Core/Game.h"

class DelimiterShapeComponent : public ShapeComponent
{
public:
	DelimiterShapeComponent();

	void Draw() override;
};


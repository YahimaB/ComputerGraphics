#pragma once

#include "GameComponent.h"

class Actor
{
public:
	Actor();
	~Actor();

	bool Initialize();
	void Draw();
	void Update();
	void DestroyResources();

private:
	static const int MAX_COMPONENTS = 10;
	GameComponent* components_[MAX_COMPONENTS];

};


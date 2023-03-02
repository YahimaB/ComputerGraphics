#pragma once

#include "GameComponent.h"

class Actor
{
public:
	Actor();
	~Actor();

	bool Initialize();
	void Update();
	void Draw();
	void DestroyResources();

private:
	static const int MAX_COMPONENTS = 10;
	GameComponent* components_[MAX_COMPONENTS];

};


#pragma once

#include <d3d.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>

class Game;

class GameComponent
{
	int _temp;

public:
	GameComponent();
	~GameComponent();

	bool IsUniqueVariant = false;

public:
	

	void virtual Initialize() {};
	void virtual Draw() {};
	void virtual Update(float deltaTime) {};
	void virtual DestroyResources() {};


protected:
	Game* game;
};


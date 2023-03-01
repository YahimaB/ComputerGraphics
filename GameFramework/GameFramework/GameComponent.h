#pragma once
#include "Game.h"

class GameComponent
{
public:
	GameComponent();
	~GameComponent();

	bool Initialize();
	void Reload();
	void Draw();
	void Update();
	void DestroyResources();


private:
	Game* game;
};


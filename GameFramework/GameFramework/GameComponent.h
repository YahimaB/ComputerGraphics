#pragma once

#include <d3d.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>

class Game;

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

	Game* game;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;


private:
	

};


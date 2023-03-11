#include "GameComponent.h"
#include "Game.h"

GameComponent::GameComponent()
{
	game = Game::Instance;
}

GameComponent::~GameComponent()
{
	game = nullptr;
}

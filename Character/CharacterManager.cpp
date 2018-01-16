#include "../stdafx.h"
#include "CharacterManager.h"
#include "Player.h"
#include "Enemy.h"

CharacterManager::CharacterManager()
{
}

CharacterManager::~CharacterManager()
{
	SAFE_DELETE(monster);
	SAFE_DELETE(player);
}

void CharacterManager::CreateObject()
{
	player = new Player();
	monster = new Enemy(L"mutant");
}

void CharacterManager::Update()
{
	player->Update();
	playerLocation = player->GetLocation();
	monster->SetPlayerLocation(playerLocation);

	monster->Update();
	enemyLocation = monster->GetLocation();
}

void CharacterManager::Render()
{
	player->Render();
	monster->Render();
}

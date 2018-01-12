#include "../stdafx.h"
#include "CharacterManager.h"
#include "Player.h"
#include "Enemy.h"

int CharacterManager::monsterCount = NULL;

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
	//monster->LinkPlayer(player);
}

void CharacterManager::CheckMonsterCreate()
{
	monsterCount++;
}

void CharacterManager::Update()
{
	player->Update();
	playerLocation = player->GetLocation();
	monster->Update();
}

void CharacterManager::Render()
{
	player->Render();
	monster->Render();
}

void CharacterManager::SetLocation(wstring className, D3DXVECTOR3 location)
{
	//TODO 몬스터가 여러마리 생성될 때 각 몬스터의 로케이션 좌표를 어떻게 저장할 것인지?
	if (className == L"Player")
	{
		this->location.push_back(location);
	}
	else
	{
		//monsterLocation = *location;
	}


}

D3DXVECTOR3 CharacterManager::GetLocation(wstring className)
{
	if (className == L"Player")
		int a = 0;
	//return monsterLocation;
	else
		return playerLocation;

	return D3DXVECTOR3(0, 0, 0);
}

#pragma once

class Player;
class Enemy;
class CharacterManager
{
public:
	CharacterManager();
	~CharacterManager();

	void CreateObject();

	void Update();
	void Render();

private:
	Player* player;
	Enemy* monster;

private:
	D3DXVECTOR3 playerLocation;
	D3DXVECTOR3 enemyLocation;
};

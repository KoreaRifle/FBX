#pragma once

class ModelScene;
//class CollisionBox;
class Player;
class Enemy
{
public:
	Enemy(wstring enemyFbxFileName);
	~Enemy();

	//void LinkPlayer(Player* player) { this->player = player; }
	
	void Update();
	void Render();

private:
	ModelScene* model;
	//CollisionBox* colBox;
	Player* player;

private:
	wstring className;

	wstring tPosePath;
	wstring idlePath;
	wstring runPath;
	wstring combatReadyPath;
	wstring attackPath;
	wstring deathPath;

	D3DXMATRIX world;

	D3DXVECTOR3 location;
	float rotationAngle;;

	D3DXVECTOR3 collisionBoxMin, collisionBoxMax;

private:
	//Player* player;
	D3DXVECTOR3 playerLocation;

	bool isDistanceArea;
};
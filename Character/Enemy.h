#pragma once

class ModelScene;
//class CollisionBox;
class Player;
class Enemy
{
public:
	Enemy(wstring enemyFbxFileName);
	~Enemy();

	D3DXVECTOR3 GetLocation() { return location; }
	void SetPlayerLocation(D3DXVECTOR3 playerLocation) { this->playerLocation = playerLocation; }
	
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
	float moveSpeed;
	float rotationAngle;;

	D3DXVECTOR3 collisionBoxMin, collisionBoxMax;

private:
	//Player* player;
	D3DXVECTOR3 playerLocation;

	bool isDistanceArea;
	bool isRunTrigger;
};
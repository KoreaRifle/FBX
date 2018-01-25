#pragma once

class ModelScene;
class CollisionBox;
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
	CollisionBox* colBox;

	bool isCollider;

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

	D3DXVECTOR3 collisionBoxMin, collisionBoxMax;

	float rotationAngle;
	float prevRotationRange;
	D3DXVECTOR3 normalDistance;
	D3DXVECTOR3 distance;
	D3DXVECTOR3 prevDistance;

private:
	D3DXVECTOR3 playerLocation;

	bool isDistanceArea;
	bool isRunTrigger;
	bool isAttackTrigger;
};
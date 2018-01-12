#pragma once
#include "CharacterManager.h"

class ModelScene;
class CollisionBox;
class Player : public CharacterManager
{
public:
	Player();
	~Player();

	void Update();
	void Render();

	D3DXVECTOR3 GetLocation() { return location; }

private:
	ModelScene* model;
	CollisionBox* colBox;

private:
	wstring className;

	wstring tPosePath;
	wstring idlePath;
	wstring runPath;
	wstring attackPath;
	wstring jumpPath;

	bool isMoving;
	bool isAttack;
	bool isTrigger;
	bool isJump;

	D3DXMATRIX world;

	D3DXVECTOR3 location;
	float rotationAngle;

	D3DXVECTOR3 collisionBoxMin, collisionBoxMax;
};


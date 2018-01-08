#pragma once

class ModelScene;
class CollisionBox;
class Player
{
public:
	Player();
	~Player();

	void Update();
	void Render();


private:
	ModelScene* model;
	CollisionBox* colBox;

private:

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


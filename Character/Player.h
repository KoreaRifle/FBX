#pragma once
#include "CharacterManager.h"

class ModelScene;
class CollisionBox;
class Player
{
public:
	Player();
	~Player();

	void Update();
	void Render();

	D3DXVECTOR3 GetLocation() { return location; }
	void SetMonsterLocation(D3DXVECTOR3 monsterLocation) { this->monsterLocation = monsterLocation; }

// Collider 생성
private:
	ModelScene* model;
	CollisionBox* colBox;

	bool isCollider;

private:
	wstring animationPath;
	wstring weaponPath;

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

	float moveSpeed;
	D3DXVECTOR3 location;
	float rotationAngle;

	D3DXVECTOR3 collisionBoxMin, collisionBoxMax;

private:
	D3DXVECTOR3 monsterLocation;

// 검 관련
public:

private:
	ModelScene* swordModel;
	D3DXMATRIX matWeapon; // swordJoint 매트릭스 저장
	bool isSwapWeapon;
	int isSwapWeaponNumber;
};


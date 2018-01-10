#pragma once

class ModelScene;
//class CollisionBox;
class Enemy
{
public:
	Enemy(wstring enemyFbxFileName);
	~Enemy();
	
	void Update();
	void Render();

private:
	ModelScene* model;
	//CollisionBox* colBox;

private:
	wstring tPosePath;
	wstring idlePath;
	wstring runPath;
	wstring attackPath;
	wstring jumpPath;

	D3DXMATRIX world;

	D3DXVECTOR3 location;
	float rotationAngle;;

	D3DXVECTOR3 collisionBoxMin, collisionBoxMax;
};
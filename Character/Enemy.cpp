#include "../stdafx.h"
#include "Enemy.h"
#include "../Model/ModelScene.h"
//#include "CollisionBox.h"

//TODO Enemy Ŭ���� �ڵ� �� �����ϱ� ������ CollisionBox �ּ� Ǯ�� ����
Enemy::Enemy(wstring enemyFbxFileName)
{
	wstring filePath = L"./Contents/Models/";
	wstring animationPath = filePath + L"Sword and Shield Pack/";

	tPosePath = filePath + L"Enemy/" + enemyFbxFileName;
	idlePath = animationPath + L"sword and shield idle.fbx";
	runPath = animationPath + L"sword and shield run.fbx";
	attackPath = animationPath + L"sword and shield slash.fbx";
	jumpPath = animationPath + L"sword and shield jump.fbx";

	model = new ModelScene();

	model->SetRootFilePath(animationPath);
	
	model->LoadScene(tPosePath, true, true, true, false);
	model->LoadScene(idlePath, false, false, false, true);
	model->LoadScene(runPath, false, false, false, true);

	// Default Animation ����
	model->SetCurrentAnimation(idlePath);

	D3DXMatrixIdentity(&world);
	location = D3DXVECTOR3(0, 0, 200);
}

Enemy::~Enemy()
{
	//SAFE_DELETE(colBox);
	SAFE_DELETE(model);
}

void Enemy::Update()
{
}

void Enemy::Render()
{
}

#include "../stdafx.h"
#include "Enemy.h"
#include "../Model/ModelScene.h"
//#include "CollisionBox.h"

//TODO Enemy 클래스 코드 다 구현하기 전에는 CollisionBox 주석 풀지 말것
Enemy::Enemy(wstring enemyFbxFileName)
{
	className = enemyFbxFileName;
	wstring filePath = L"./Contents/Models/";
	wstring animationPath = filePath + L"Animation/";

	tPosePath = filePath + L"Enemy/" + enemyFbxFileName + L".fbx";
	idlePath = animationPath + L"Idle.fbx";
	runPath = animationPath + L"Run.fbx";
	combatReadyPath = animationPath + L"CombatReady.fbx";
	attackPath = animationPath + L"PunchAttack.fbx";
	deathPath = animationPath + L"Death.fbx";


	model = new ModelScene();

	model->SetRootFilePath(animationPath);
	
	model->LoadScene(tPosePath, true, true, true, false);
	model->LoadScene(idlePath, false, false, false, true);
	model->LoadScene(runPath, false, false, false, true);
	model->LoadScene(combatReadyPath, false, false, false, true);
	model->LoadScene(attackPath, false, false, false, true);
	model->LoadScene(deathPath, false, false, false, true);

	// Default Animation 설정
	model->SetCurrentAnimation(idlePath);

	D3DXMatrixIdentity(&world);
	location = D3DXVECTOR3(0, 0, 200);

	//player = new Player();
}

Enemy::~Enemy()
{
	//SAFE_DELETE(player);
	//SAFE_DELETE(colBox);
	SAFE_DELETE(model);
}

void Enemy::Update()
{
	D3DXMATRIX matTranslation, matRotation;
	D3DXMatrixTranslation(&matTranslation, location.x, location.y, location.z);

	//? matRotation은 현재 테스트용으로 플레이어를 바라보도록 세팅해놓음
	float rotationAngle = (float)D3DX_PI;
	D3DXMatrixRotationAxis(&matRotation, &D3DXVECTOR3(0, 1, 0), rotationAngle);

	world = matRotation * matTranslation;

	model->SetWorldTransform(world);

	// 플레이어와 몬스터간의 거리 계산
	//playerLocation = player->GetLocation();
	D3DXVECTOR3 distance;
	distance.x = location.x - playerLocation.x;
	distance.z = location.z - playerLocation.z;

	if (isDistanceArea == false)
	{
		if (distance.x <= 5.0f && distance.x >= -5.0f)
		{
			if (distance.z <= 5.0f && distance.z >= -5.0f)
			{
				isDistanceArea = true;
				model->SetCurrentAnimation(combatReadyPath);
			}
		}
	}

	model->Update();
}

void Enemy::Render()
{
	model->Render();
}

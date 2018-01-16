#include "../stdafx.h"
#include "Enemy.h"
#include "../Model/ModelScene.h"
//#include "CollisionBox.h"

//TODO Enemy 클래스 코드 다 구현하기 전에는 CollisionBox 주석 풀지 말것
Enemy::Enemy(wstring enemyFbxFileName)
	: isDistanceArea(false), moveSpeed(100.0f)
	, isRunTrigger(false)
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
	model->LoadScene(idlePath, false, false, false, true, true);
	model->LoadScene(runPath, false, false, false, true, true);
	model->LoadScene(combatReadyPath, false, false, false, true);
	model->LoadScene(attackPath, false, false, false, true);
	model->LoadScene(deathPath, false, false, false, true);

	// Default Animation 설정
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
	// 몹이 보고 있는 방향으로 부채꼴 콜리전 생성하여 거기에 닿으면 움직이도록 수정예정

	//float crDistance = 150.0f; // combatReadyDistance : 전투애니메이션 유효거리
	//float czDistance = 100.0f; // combatZoneDistance : 몬스터가 따라오는 유효거리
	
	// 플레이어와 몬스터간의 거리 계산
	//D3DXVECTOR3 distance;
	//distance.x = location.x - playerLocation.x;
	//distance.z = location.z - playerLocation.z;

	//if (isDistanceArea == false)
	//{
	//	isRunTrigger = false;
	//	if (distance.x <= crDistance && distance.x >= -crDistance)
	//	{
	//		if (distance.z <= crDistance && distance.z >= -crDistance)
	//		{
	//			isDistanceArea = true;
	//			model->SetCurrentAnimation(combatReadyPath);
	//		}
	//	}
	//}
	//// 플레이어가 combatZone에 진입했을 때(몬스터가 플레이어 위치로 이동)
	//else if (isDistanceArea == true)
	//{
	//	if (distance.x <= czDistance && distance.x >= -czDistance)
	//	{
	//		if (distance.z <= czDistance && distance.z >= -czDistance)
	//		{
	//			if (isRunTrigger == false)
	//			{
	//				model->SetCurrentAnimation(runPath);
	//				isRunTrigger = true;
	//			}
	//			// 몬스터가 플레이어 위치로 이동함
	//			if (abs(playerLocation.z) >= abs(location.z))
	//			{
	//				if (playerLocation.z > 0)
	//					location.z = location.z + (location.z * Frames::Get()->TimeElapsed());
	//				else
	//					location.z = location.z - (location.z * Frames::Get()->TimeElapsed());

	//				//TODO combatZone을 벗어났을 때 combatReady 상태로 전환 필요
	//			}
	//			else if (abs(playerLocation.z <= abs(location.z)))
	//			{
	//				if (playerLocation.z > 0)
	//					location.z = location.z - (location.z * Frames::Get()->TimeElapsed());
	//				else
	//					location.z = location.z + (location.z * Frames::Get()->TimeElapsed());
	//			}
	//		}
	//	}
	//	else
	//	{
	//		isRunTrigger = false;
	//		model->SetCurrentAnimation(combatReadyPath);
	//	}
	//}
	//// 플레이어와 몬스터 거리가 crDistance를 벗어났을 때
	//else if (isDistanceArea == true)
	//{
	//	if (distance.x > crDistance)
	//	{
	//		isDistanceArea = false;
	//		model->SetCurrentAnimation(idlePath);
	//	}
	//	else if(distance.x < -crDistance)
	//	{
	//		isDistanceArea = false;
	//		model->SetCurrentAnimation(idlePath);
	//	}
	//	else if (distance.z > crDistance)
	//	{
	//		isDistanceArea = false;
	//		model->SetCurrentAnimation(idlePath);
	//	}
	//	else if (distance.z < -crDistance)
	//	{
	//		isDistanceArea = false;
	//		model->SetCurrentAnimation(idlePath);
	//	}
	//}

	D3DXMATRIX matTranslation, matRotation;
	D3DXMatrixTranslation(&matTranslation, location.x, location.y, location.z);

	//? matRotation은 현재 테스트용으로 플레이어를 바라보도록 세팅해놓음
	float rotationAngle = (float)D3DX_PI;
	D3DXMatrixRotationAxis(&matRotation, &D3DXVECTOR3(0, 1, 0), rotationAngle);

	world = matRotation * matTranslation;

	model->SetWorldTransform(world);

	model->Update();
}

void Enemy::Render()
{
	model->Render();
}

#include "../stdafx.h"
#include "Enemy.h"
#include "../Model/ModelScene.h"
//#include "CollisionBox.h"

//TODO Enemy Ŭ���� �ڵ� �� �����ϱ� ������ CollisionBox �ּ� Ǯ�� ����
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
	// ���� ���� �ִ� �������� ��ä�� �ݸ��� �����Ͽ� �ű⿡ ������ �����̵��� ��������

	//float crDistance = 150.0f; // combatReadyDistance : �����ִϸ��̼� ��ȿ�Ÿ�
	//float czDistance = 100.0f; // combatZoneDistance : ���Ͱ� ������� ��ȿ�Ÿ�
	
	// �÷��̾�� ���Ͱ��� �Ÿ� ���
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
	//// �÷��̾ combatZone�� �������� ��(���Ͱ� �÷��̾� ��ġ�� �̵�)
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
	//			// ���Ͱ� �÷��̾� ��ġ�� �̵���
	//			if (abs(playerLocation.z) >= abs(location.z))
	//			{
	//				if (playerLocation.z > 0)
	//					location.z = location.z + (location.z * Frames::Get()->TimeElapsed());
	//				else
	//					location.z = location.z - (location.z * Frames::Get()->TimeElapsed());

	//				//TODO combatZone�� ����� �� combatReady ���·� ��ȯ �ʿ�
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
	//// �÷��̾�� ���� �Ÿ��� crDistance�� ����� ��
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

	//? matRotation�� ���� �׽�Ʈ������ �÷��̾ �ٶ󺸵��� �����س���
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

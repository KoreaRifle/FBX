#include "../stdafx.h"
#include "Enemy.h"
#include "../Model/ModelScene.h"
#include "CollisionBox.h"

//TODO Enemy Ŭ���� �ڵ� �� �����ϱ� ������ CollisionBox �ּ� Ǯ�� ����
Enemy::Enemy(wstring enemyFbxFileName)
	: isDistanceArea(false), moveSpeed(100.0f)
	, isRunTrigger(false), isAttackTrigger(false), isCollider(false)
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

	//TODO ������ġ �����ϰ�
	location = D3DXVECTOR3(0, 0, 200);
	playerLocation = D3DXVECTOR3(0, 0, 0);

	model->GetCollisionBoxMinMaxValue(&collisionBoxMin, &collisionBoxMax);
	colBox = new CollisionBox();

	rotationAngle = 0.0f;

	distance = D3DXVECTOR3(0, 0, 0);
}

Enemy::~Enemy()
{
	SAFE_DELETE(colBox);
	SAFE_DELETE(model);
}

void Enemy::Update()
{
	//TODO ���� ���� �ִ� �������� ��ä�� �ݸ��� �����Ͽ� �ű⿡ ������ �����̵��� ��������

	float crDistance = 150.0f; // combatReadyDistance : �����ִϸ��̼� ��ȿ�Ÿ�
	float czDistance = 100.0f; // combatZoneDistance : ���Ͱ� ������� ��ȿ�Ÿ�
	
	// �÷��̾�� ���Ͱ��� �Ÿ� ���
	distance.x = playerLocation.x - location.x;
	distance.z = playerLocation.z - location.z;

	if (isDistanceArea == false)
	{
		isRunTrigger = false;
		if (distance.x <= crDistance && distance.x >= -crDistance)
		{
			if (distance.z <= crDistance && distance.z >= -crDistance)
			{
				isDistanceArea = true;
				model->SetCurrentAnimation(combatReadyPath);
			}
		}
	}
	// �÷��̾ combatZone�� �������� ��(���Ͱ� �÷��̾� ��ġ�� �̵�)
	else if (isDistanceArea == true)
	{
		if (distance.x <= czDistance && distance.x >= -czDistance)
		{
			if (distance.z <= czDistance && distance.z >= -czDistance)
			{
				if (isRunTrigger == false)
				{
					model->SetCurrentAnimation(runPath);
					isRunTrigger = true;
				}

				//? ���Լ��� ������ ���� �� �ִ�.
				//? f(x) = y
				//? f-1(y) = x
				//? ������ ���� ����(theta)�� ���� �� theta�� acos(x*y/|x||y|) �̹Ƿ�
				//? ����ȭ(Normalize)�� ���� |x||y|�� 1�� ó���ع�����(����ϱⰡ ���� ������)
				float dotDistance; // ������
				D3DXVec3Normalize(&normalDistance, &distance); // distance�� 1�� ����ȭ
				// z���Ⱚ�� distance ���� �������ش�.
				dotDistance = D3DXVec3Dot(&D3DXVECTOR3(0, 0, 1), &normalDistance);
				rotationAngle = acosf(dotDistance); // ������ ���� �ڻ����� ���Լ�(��ũ�ڻ���)�� ���� ������ ���Ѵ�.

				if (distance.x < 0) rotationAngle = -rotationAngle;
				
				// �ٶ󺸰� �ִ� �������� ������
				float accessRange = 15.0f;
				if (location.z > playerLocation.z + accessRange) location.z -= 0.5f;
				else if (location.z < playerLocation.z - accessRange) location.z += 0.5f;

				if (location.x > playerLocation.x + accessRange) location.x -= 0.5f;
				else if (location.x < playerLocation.x - accessRange) location.x += 0.5f;

				// ���� ���Ͱ� �÷��̾� ��ġ�� �����Ÿ� �̻� �������� ��� ���ݸ������ ����
				D3DXVECTOR2 playerRangeMin, playerRangeMax;
				playerRangeMin.x = playerLocation.x - accessRange;
				playerRangeMin.y = playerLocation.z - accessRange;
				playerRangeMax.x = playerLocation.x + accessRange;
				playerRangeMax.y = playerLocation.z + accessRange;

				if (location.x >= playerRangeMin.x && location.x <= playerRangeMax.x)
				{
					if (location.z >= playerRangeMin.y && location.z <= playerRangeMax.y)
					{
						if (isAttackTrigger == false)
						{
							isAttackTrigger = true;
							model->SetCurrentAnimation(attackPath);
						}
					}
				}
				if (location.x <= playerRangeMin.x || location.x >= playerRangeMax.x)
				{
					if (location.z <= playerRangeMin.y || location.z >= playerRangeMax.y)
					{
						if (isAttackTrigger == true)
						{
							isAttackTrigger = false;
							model->SetCurrentAnimation(runPath);
						}
					}
				}
			}
		}
		else
		{
			isAttackTrigger = false;
			isRunTrigger = false;
			model->SetCurrentAnimation(combatReadyPath);
		}
	}
	// �÷��̾�� ���� �Ÿ��� crDistance�� ����� ��
	else if (isDistanceArea == true)
	{
		if (distance.x > crDistance)
		{
			isDistanceArea = false;
			model->SetCurrentAnimation(idlePath);
		}
		else if(distance.x < -crDistance)
		{
			isDistanceArea = false;
			model->SetCurrentAnimation(idlePath);
		}
		else if (distance.z > crDistance)
		{
			isDistanceArea = false;
			model->SetCurrentAnimation(idlePath);
		}
		else if (distance.z < -crDistance)
		{
			isDistanceArea = false;
			model->SetCurrentAnimation(idlePath);
		}
	}

	D3DXMATRIX matScaling, matTranslation, matRotation;
	D3DXMatrixScaling(&matScaling, 0.1f, 0.1f, 0.1f);
	D3DXMatrixTranslation(&matTranslation, location.x, location.y, location.z);
	D3DXMatrixRotationAxis(&matRotation, &D3DXVECTOR3(0, 1, 0), rotationAngle);

	world = matScaling * matRotation * matTranslation;
	model->SetWorldTransform(world);

	if (isCollider == false)
	{
		isCollider = true;
		model->GetCollisionBoxMinMaxValue(&collisionBoxMin, &collisionBoxMax);
		colBox->CalcCollider(collisionBoxMin, collisionBoxMax);
	}
	colBox->SetWorldMatrix(world);

	model->Update();
	if (isCollider == true)colBox->Update();
}

void Enemy::Render()
{
	model->Render();
	if (isCollider == true) colBox->Render();
}

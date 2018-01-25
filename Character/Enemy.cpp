#include "../stdafx.h"
#include "Enemy.h"
#include "../Model/ModelScene.h"
#include "CollisionBox.h"

//TODO Enemy 클래스 코드 다 구현하기 전에는 CollisionBox 주석 풀지 말것
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

	// Default Animation 설정
	model->SetCurrentAnimation(idlePath);

	D3DXMatrixIdentity(&world);

	//TODO 생성위치 랜덤하게
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
	//TODO 몹이 보고 있는 방향으로 부채꼴 콜리전 생성하여 거기에 닿으면 움직이도록 수정예정

	float crDistance = 150.0f; // combatReadyDistance : 전투애니메이션 유효거리
	float czDistance = 100.0f; // combatZoneDistance : 몬스터가 따라오는 유효거리
	
	// 플레이어와 몬스터간의 거리 계산
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
	// 플레이어가 combatZone에 진입했을 때(몬스터가 플레이어 위치로 이동)
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

				//? 역함수로 각도를 구할 수 있다.
				//? f(x) = y
				//? f-1(y) = x
				//? 내적에 따른 각도(theta)를 구할 때 theta는 acos(x*y/|x||y|) 이므로
				//? 정규화(Normalize)를 통해 |x||y|를 1로 처리해버린다(계산하기가 쉽기 때문에)
				float dotDistance; // 내적값
				D3DXVec3Normalize(&normalDistance, &distance); // distance를 1로 정규화
				// z방향값과 distance 값을 내적해준다.
				dotDistance = D3DXVec3Dot(&D3DXVECTOR3(0, 0, 1), &normalDistance);
				rotationAngle = acosf(dotDistance); // 내적한 값을 코사인의 역함수(아크코사인)를 통해 각도를 구한다.

				if (distance.x < 0) rotationAngle = -rotationAngle;
				
				// 바라보고 있는 방향으로 전진함
				float accessRange = 15.0f;
				if (location.z > playerLocation.z + accessRange) location.z -= 0.5f;
				else if (location.z < playerLocation.z - accessRange) location.z += 0.5f;

				if (location.x > playerLocation.x + accessRange) location.x -= 0.5f;
				else if (location.x < playerLocation.x - accessRange) location.x += 0.5f;

				// 만약 몬스터가 플레이어 위치에 일정거리 이상 근접했을 경우 공격모션으로 변경
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
	// 플레이어와 몬스터 거리가 crDistance를 벗어났을 때
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

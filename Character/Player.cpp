#include "../stdafx.h"
#include "Player.h"
#include "../Model/ModelScene.h"
#include "CollisionBox.h"

#define fPI (float)D3DX_PI // D3DX_PI를 float 형으로 캐스팅 한 것
#define MoveDegree fPI/18.0f // 캐릭터의 회전각에 따른 속도

Player::Player()
	: moveSpeed(100.0f)
{
	wstring filePath = L"./Contents/Models/";
	animationPath = filePath + L"Sword and Shield Pack/";
	weaponPath = filePath + L"Weapon/";

	tPosePath = animationPath + L"paladin_prop_j_nordstrom.fbx";
	idlePath = animationPath + L"sword and shield idle.fbx";
	runPath = animationPath + L"sword and shield run.fbx";
	attackPath = animationPath + L"sword and shield slash.fbx";
	jumpPath = animationPath + L"sword and shield jump.fbx";

	isMoving = isAttack = isTrigger = false;

	model = new ModelScene();

	model->SetRootFilePath(animationPath);

	model->LoadScene(tPosePath, true, true, true, false);

	model->LoadScene(idlePath, false, false, false, true);
	model->LoadScene(runPath, false, false, false, true, true);
	model->LoadScene(attackPath, false, false, false, true);
	model->LoadScene(jumpPath, false, false, false, true);

	// Default Animation 설정
	model->SetCurrentAnimation(idlePath);

	D3DXMatrixIdentity(&world);

	location = D3DXVECTOR3(0, 0, 0);

	UserInterface::Get()->AddFbxModelTransloate(&location.x, &location.y, &location.z);

	rotationAngle = 0.0f;
	UserInterface::Get()->AddFbxModelRotate(0, &rotationAngle, 0);

	model->GetCollisionBoxMinMaxValue(&collisionBoxMin, &collisionBoxMax);
	colBox = new CollisionBox(collisionBoxMin, collisionBoxMax);
	colBox->SetForceScale(0.1f);
}
	
Player::~Player()
{
	SAFE_DELETE(colBox);
	SAFE_DELETE(model);
}

void Player::Update()
{
	// 카메라가 따라다니게 만듬
	Camera::Get()->SetPlayerLocation(location);

	if (KEYBOARD->KeyDown('1'))
	{
		bool isSwap = model->SetChangeWeapon(weaponPath + L"SK_Blade_BlackKnight.fbx");
		model->LoadScene(weaponPath + L"SK_Blade_BlackKnight.fbx", true, false, true, false, false);
	}

	// W 누르면 캐릭터가 앞으로 움직이는 애니메이션 동작
	if (isAttack == false)
	{
		//? 라디안 각이 0~360도 일때 수행
		if (rotationAngle >= 0.0f && rotationAngle <= 2 * fPI)
		{
			// Forward, Backward
			if (KEYBOARD->KeyPress('W') || KEYBOARD->KeyPress('S'))
			{
				isMoving = true;
				if (isTrigger == false)
				{
					isTrigger = true;
					model->SetCurrentAnimation(runPath);
				}

				//? 근사치를 경계값으로 고정시킴(10도)
				//? rotationAngle을 MoveDegree(10도)만큼 증감하므로 범위는 +- 10도로 함
				if (KEYBOARD->KeyPress('W'))
				{
					if (rotationAngle > fPI)
						rotationAngle += MoveDegree;
					else if (rotationAngle <= fPI)
						rotationAngle -= MoveDegree;

					// 범위 지정 후 값 고정(떨림 방지)
					if (rotationAngle <= MoveDegree && rotationAngle >= 0.0f - MoveDegree)
						rotationAngle = 0.0f;
					location.z = location.z + (moveSpeed * Frames::Get()->TimeElapsed());
				}
				else if (KEYBOARD->KeyPress('S'))
				{
					if (rotationAngle >= 0.0f && rotationAngle < fPI)
						rotationAngle += MoveDegree;
					else if (rotationAngle > fPI && rotationAngle < 2 * fPI)
						rotationAngle -= MoveDegree;

					// 범위 지정 후 값 고정(떨림 방지)
					if (rotationAngle <= fPI + MoveDegree && rotationAngle >= fPI - MoveDegree)
						rotationAngle = fPI;
					location.z = location.z - (moveSpeed * Frames::Get()->TimeElapsed());
				}
			}
			// Right, Left
			if (KEYBOARD->KeyPress('A') || KEYBOARD->KeyPress('D'))
			{
				isMoving = true;
				if (isTrigger == false)
				{
					isTrigger = true;
					model->SetCurrentAnimation(runPath);
				}

				if (KEYBOARD->KeyPress('D'))
				{
					// rotationAngle > 270 or rotationAngle < 90
					if (rotationAngle >= 3.0f * fPI / 2.0f || rotationAngle < fPI / 2.0f)
						rotationAngle += MoveDegree;
					
					else if (rotationAngle > fPI / 2.0f && rotationAngle < 3.0f * fPI / 2.0f)
						rotationAngle -= MoveDegree;

					// 범위 지정 후 값 고정(떨림 방지)
					if (rotationAngle <= fPI / 2.0f + MoveDegree && rotationAngle >= fPI / 2.0f - MoveDegree)
						rotationAngle = fPI / 2.0f;
					location.x = location.x + (moveSpeed * Frames::Get()->TimeElapsed());
				}
				else if (KEYBOARD->KeyPress('A'))
				{
					if (rotationAngle >= fPI / 2.0f && rotationAngle < 3.0f * fPI / 2.0f) rotationAngle += MoveDegree;
					else if (rotationAngle < fPI / 2.0f || rotationAngle > 3.0f * fPI / 2.0f) rotationAngle -= MoveDegree;

					// 범위 지정 후 값 고정(떨림 방지)
					if (rotationAngle <= 3.0f * fPI / 2.0f + MoveDegree && rotationAngle >= 3.0f * fPI / 2.0f - MoveDegree)
						rotationAngle = 3.0f * fPI / 2.0f;
					location.x = location.x - (moveSpeed * Frames::Get()->TimeElapsed());
				}
			}
		}
		//? 라디안 값이 360도 초과일 땐 값을 0도로,
		//? 라디안 값이 0도 미만일 땐 값을 360도로 rotationAngle 값을 강제 조정
		else if (rotationAngle <= 0.0f || rotationAngle >= 2 * fPI)
		{
			if (rotationAngle > 2 * fPI) rotationAngle = 0.0f;
			else if (rotationAngle < 0.0f) rotationAngle = 2 * fPI;
		}

		// 키 입력 중지 상태
		if (KEYBOARD->KeyUp('W') || KEYBOARD->KeyUp('S') || KEYBOARD->KeyUp('A') || KEYBOARD->KeyUp('D'))
		{
			isMoving = false;
			isTrigger = false;
			model->SetCurrentAnimation(idlePath);
		}

		//TODO : 점프 모션 구현
		//if (KEYBOARD->KeyDown(VK_SPACE))
		//{
		//	while (isJump != false)
		//	{
		//		isAttack = false;
		//		model->SetCurrentAnimation(jumpPath);

		//		// TODO : 만약 점프 모션이 다 끝났을 경우 루프 탈출

		//	}
		//}

		// 회전 시 D3DXMatrixRotationAxis를 사용해서
		// 반환행렬(matRotation), 회전축(Y축), 회전각(rotationAngle)을 넣는다.
		D3DXMATRIX matRotation, matTranslation;
		D3DXMatrixRotationAxis(&matRotation, &D3DXVECTOR3(0, 1, 0), rotationAngle);
		D3DXMatrixTranslation(&matTranslation, location.x, location.y, location.z);

		world = matRotation * matTranslation;

		colBox->SetWorldMatrix(world);
		model->SetWorldTransform(world);
	}

	// 마우스 좌클릭 시 공격하는 애니메이션 동작
	if (isMoving == false)
	{
		if (MOUSE->ButtonDown(0))
		{
			isAttack = true;
			model->SetCurrentAnimation(attackPath);
		}
		else if (MOUSE->ButtonUp(0))
		{
			isAttack = false;
			model->SetCurrentAnimation(idlePath);
		}
	}
	colBox->SetWorldMatrix(world);

	model->Update();
	colBox->Update();
}

void Player::Render()
{
	model->Render();
	colBox->Render();
}

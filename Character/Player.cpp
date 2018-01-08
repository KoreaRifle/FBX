#include "../stdafx.h"
#include "Player.h"
#include "../Model/ModelScene.h"
#include "../Character/CollisionBox.h"

// TODO : 콜리전 클래스 불러올 때 SetWorldMatrix() 함수를 통해 player의 매트릭스 전달해야함
Player::Player()
{
	wstring filePath = L"./Contents/Models/";
	wstring paladinFolderPath = filePath + L"Sword and Shield Pack/";

	tPosePath = paladinFolderPath + L"paladin_prop_j_nordstrom.fbx";
	idlePath = paladinFolderPath + L"sword and shield idle.fbx";
	runPath = paladinFolderPath + L"sword and shield run.fbx";
	attackPath = paladinFolderPath + L"sword and shield slash.fbx";
	jumpPath = paladinFolderPath + L"sword and shield jump.fbx";

	isMoving = isAttack = isTrigger = false;

	model = new ModelScene();

	model->SetRootFilePath(paladinFolderPath);

	model->LoadScene(tPosePath, true, true, true, false);
	model->LoadScene(idlePath, false, false, false, true);
	model->LoadScene(runPath, false, false, false, true);
	model->LoadScene(attackPath, false, false, false, true);
	model->LoadScene(jumpPath, false, false, false, true);

	// Default Animation 설정
	model->SetCurrentAnimation(idlePath);

	D3DXMatrixIdentity(&world);

	location = D3DXVECTOR3(0, 0, 0);

	UserInterface::Get()->AddFbxModelTransloate(&location.x, &location.y, &location.z);

	// 회전각을 라디안으로 계산
	//rotationAngle = (float)D3DX_PI / 180;
	rotationAngle = 0.0f;
	UserInterface::Get()->AddFbxModelRotate(0, &rotationAngle, 0);

	model->GetCollisionBoxMinMaxValue(&collisionBoxMin, &collisionBoxMax);
	colBox = new CollisionBox(collisionBoxMin, collisionBoxMax);
}
	
Player::~Player()
{
	SAFE_DELETE(model);
}

void Player::Update()
{
	// 카메라가 따라다니게 만듬
	Camera::Get()->SetPlayerLocation(location);


	// W 누르면 캐릭터가 앞으로 움직이는 애니메이션 동작
	// TODO : 캐릭터가 바라보고 있는 방향 찾아서 rotationAngle 값 조정
	if (isAttack == false)
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

			if (KEYBOARD->KeyPress('W'))
			{
				if (rotationAngle < 0.0f) rotationAngle += 6.0f;
				else if (rotationAngle > 0.0f) rotationAngle -= 6.0f;
				location.z++;
			}
			if (KEYBOARD->KeyPress('S'))
			{
				location.z--;
				if (rotationAngle < 180.0f) rotationAngle += 6.0f;
				else if (rotationAngle > 180.0f) rotationAngle -= 6.0f;
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
				if (rotationAngle < 90.0f) rotationAngle += 6.0f;
				else if (rotationAngle > 90.0f) rotationAngle -= 6.0f;
				location.x++;
			}
			if (KEYBOARD->KeyPress('A'))
			{
				if (rotationAngle < 270.0f) rotationAngle += 6.0f;
				else if (rotationAngle > 270.0f) rotationAngle -= 6.0f;
				location.x--;
			}
		}
		// 키 입력 중지 상태
		if (KEYBOARD->KeyUp('W') || KEYBOARD->KeyUp('S') || KEYBOARD->KeyUp('A') || KEYBOARD->KeyUp('D'))
		{
			isMoving = false;
			isTrigger = false;
			model->SetCurrentAnimation(idlePath);
		}

		if (KEYBOARD->KeyDown(VK_SPACE))
		{
			while (isJump != false)
			{
				isAttack = false;
				model->SetCurrentAnimation(jumpPath);

				// TODO : 만약 점프 모션이 다 끝났을 경우 루프 탈출

			}
		}

		// 회전 시 D3DXMatrixRotationAxis를 사용해서
		// 반환행렬(matRotation), 회전축(Y축), 회전각(rotationAngle)을 넣는다.


		D3DXMATRIX matRotation, matTranslation;
		D3DXMatrixRotationAxis(&matRotation, &D3DXVECTOR3(0, 1, 0), rotationAngle * 2 * Frames::Get()->TimeElapsed());
		//D3DXMatrixRotationYawPitchRoll(&matRotation, rotationAngle, 0, 0);
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

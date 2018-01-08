#include "../stdafx.h"
#include "Player.h"
#include "../Model/ModelScene.h"
#include "../Character/CollisionBox.h"

// TODO : �ݸ��� Ŭ���� �ҷ��� �� SetWorldMatrix() �Լ��� ���� player�� ��Ʈ���� �����ؾ���
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

	// Default Animation ����
	model->SetCurrentAnimation(idlePath);

	D3DXMatrixIdentity(&world);

	location = D3DXVECTOR3(0, 0, 0);

	UserInterface::Get()->AddFbxModelTransloate(&location.x, &location.y, &location.z);

	// ȸ������ �������� ���
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
	// ī�޶� ����ٴϰ� ����
	Camera::Get()->SetPlayerLocation(location);


	// W ������ ĳ���Ͱ� ������ �����̴� �ִϸ��̼� ����
	// TODO : ĳ���Ͱ� �ٶ󺸰� �ִ� ���� ã�Ƽ� rotationAngle �� ����
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
		// Ű �Է� ���� ����
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

				// TODO : ���� ���� ����� �� ������ ��� ���� Ż��

			}
		}

		// ȸ�� �� D3DXMatrixRotationAxis�� ����ؼ�
		// ��ȯ���(matRotation), ȸ����(Y��), ȸ����(rotationAngle)�� �ִ´�.


		D3DXMATRIX matRotation, matTranslation;
		D3DXMatrixRotationAxis(&matRotation, &D3DXVECTOR3(0, 1, 0), rotationAngle * 2 * Frames::Get()->TimeElapsed());
		//D3DXMatrixRotationYawPitchRoll(&matRotation, rotationAngle, 0, 0);
		D3DXMatrixTranslation(&matTranslation, location.x, location.y, location.z);

		world = matRotation * matTranslation;

		colBox->SetWorldMatrix(world);
		model->SetWorldTransform(world);
	}

	// ���콺 ��Ŭ�� �� �����ϴ� �ִϸ��̼� ����
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

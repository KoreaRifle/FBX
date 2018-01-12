#include "../stdafx.h"
#include "Player.h"
#include "../Model/ModelScene.h"
#include "CollisionBox.h"

#define fPI (float)D3DX_PI // D3DX_PI�� float ������ ĳ���� �� ��
#define MoveDegree fPI/18.0f // ĳ������ ȸ������ ���� �ӵ�

Player::Player()
{
	className = L"Player";
	wstring filePath = L"./Contents/Models/";
	wstring animationPath = filePath + L"Sword and Shield Pack/";

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
	model->LoadScene(runPath, false, false, false, true);
	model->LoadScene(attackPath, false, false, false, true);
	model->LoadScene(jumpPath, false, false, false, true);

	// Default Animation ����
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
	// ī�޶� ����ٴϰ� ����
	Camera::Get()->SetPlayerLocation(location);

	// W ������ ĳ���Ͱ� ������ �����̴� �ִϸ��̼� ����
	if (isAttack == false)
	{
		//? ���� ���� 0~360�� �϶� ����
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

				//? �ٻ�ġ�� ��谪���� ������Ŵ(10��)
				//? rotationAngle�� MoveDegree(10��)��ŭ �����ϹǷ� ������ +- 10���� ��
				if (KEYBOARD->KeyPress('W'))
				{
					if (rotationAngle > fPI)
						rotationAngle += MoveDegree;
					else if (rotationAngle <= fPI)
						rotationAngle -= MoveDegree;

					// ���� ���� �� �� ����(���� ����)
					if (rotationAngle <= MoveDegree && rotationAngle >= 0.0f - MoveDegree)
						rotationAngle = 0.0f;
					location.z++;
				}
				else if (KEYBOARD->KeyPress('S'))
				{
					if (rotationAngle >= 0.0f && rotationAngle < fPI)
						rotationAngle += MoveDegree;
					else if (rotationAngle > fPI && rotationAngle < 2 * fPI)
						rotationAngle -= MoveDegree;

					// ���� ���� �� �� ����(���� ����)
					if (rotationAngle <= fPI + MoveDegree && rotationAngle >= fPI - MoveDegree)
						rotationAngle = fPI;
					location.z--;
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

					// ���� ���� �� �� ����(���� ����)
					if (rotationAngle <= fPI / 2.0f + MoveDegree && rotationAngle >= fPI / 2.0f - MoveDegree)
						rotationAngle = fPI / 2.0f;
					location.x++;
				}
				else if (KEYBOARD->KeyPress('A'))
				{
					if (rotationAngle >= fPI / 2.0f && rotationAngle < 3.0f * fPI / 2.0f) rotationAngle += MoveDegree;
					else if (rotationAngle < fPI / 2.0f || rotationAngle > 3.0f * fPI / 2.0f) rotationAngle -= MoveDegree;

					// ���� ���� �� �� ����(���� ����)
					if (rotationAngle <= 3.0f * fPI / 2.0f + MoveDegree && rotationAngle >= 3.0f * fPI / 2.0f - MoveDegree)
						rotationAngle = 3.0f * fPI / 2.0f;
					location.x--;
				}
			}
		}
		//? ���� ���� 360�� �ʰ��� �� ���� 0����,
		//? ���� ���� 0�� �̸��� �� ���� 360���� rotationAngle ���� ���� ����
		else if (rotationAngle <= 0.0f || rotationAngle >= 2 * fPI)
		{
			if (rotationAngle > 2 * fPI) rotationAngle = 0.0f;
			else if (rotationAngle < 0.0f) rotationAngle = 2 * fPI;
		}

		// Ű �Է� ���� ����
		if (KEYBOARD->KeyUp('W') || KEYBOARD->KeyUp('S') || KEYBOARD->KeyUp('A') || KEYBOARD->KeyUp('D'))
		{
			isMoving = false;
			isTrigger = false;
			model->SetCurrentAnimation(idlePath);
		}

		//TODO : ���� ��� ����
		//if (KEYBOARD->KeyDown(VK_SPACE))
		//{
		//	while (isJump != false)
		//	{
		//		isAttack = false;
		//		model->SetCurrentAnimation(jumpPath);

		//		// TODO : ���� ���� ����� �� ������ ��� ���� Ż��

		//	}
		//}

		// ȸ�� �� D3DXMatrixRotationAxis�� ����ؼ�
		// ��ȯ���(matRotation), ȸ����(Y��), ȸ����(rotationAngle)�� �ִ´�.
		D3DXMATRIX matRotation, matTranslation;
		D3DXMatrixRotationAxis(&matRotation, &D3DXVECTOR3(0, 1, 0), rotationAngle);
		D3DXMatrixTranslation(&matTranslation, location.x, location.y, location.z);

		world = matRotation * matTranslation;

		// ��ӹ��� Ŭ������ ��ġ�� �����ش�.
		SetLocation(className, location);

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

#include "stdafx.h"
#include "GameMain.h"
#include "./Render/RenderTexture.h"
#include "./Render/D2DRender.h"
#include "./Landscape/Landscape.h"
#include "./Landscape/Skydome.h"
#include "./Model/ModelScene.h"

void GameMain::Initialize()
{
	UserInterface::Get();
	Shader::CreateBuffer();
	
	renderTexture = new RenderTexture();
	skydome = new Skydome();
	landscape = new Landscape();

	wstring filePath = L"./Contents/Models/Sword and Shield Pack/";

	tPosePath = filePath + L"paladin_prop_j_nordstrom.fbx";
	idlePath  = filePath + L"sword and shield idle.fbx";
	runPath = filePath + L"sword and shield run.fbx";
	attackPath = filePath + L"sword and shield slash.fbx";
	turnPath = filePath + L"sword and shield turn.fbx";

	isMoving = isAttack = isTrigger = false;

	model = new ModelScene();

	model->SetRootFilePath(filePath);

	model->LoadScene(tPosePath, true, true, true, false);
	model->LoadScene(idlePath, false, false, false, true);
	model->LoadScene(runPath, false, false, false, true);
	model->LoadScene(attackPath, false, false, false, true);
	model->LoadScene(turnPath, false, false, false, true);

	// Default Animation ����
	model->SetCurrentAnimation(idlePath);

	D3DXMatrixIdentity(&world);

	location = D3DXVECTOR3(0, 0, 0);
	//rotation = D3DXVECTOR3(0, 0, 0);

	UserInterface::Get()->AddFbxModelTransloate(&location.x, &location.y, &location.z);

	//D3DXVec3Normalize(&rotationNormalize, &rotation);
	//UserInterface::Get()->AddFbxModelRotate(&rotationNormalize.x, &rotationNormalize.y, &rotationNormalize.z);
	
	// ȸ������ �������� ���
	//rotationAngle = (float)D3DX_PI / 180;
	rotationAngle = 0.0f;
	//playerDirection = 0.0f;
	UserInterface::Get()->AddFbxModelRotate(0, &rotationAngle, 0);
	//UserInterface::Get()->AddTempRotationValue(&playerDirection);
	scale = 0.1f;
}

void GameMain::Destroy()
{
	SAFE_DELETE(model);

	SAFE_DELETE(landscape);
	SAFE_DELETE(skydome);
	SAFE_DELETE(renderTexture);

	Shader::DeleteBuffer();
	UserInterface::Delete();
}

void GameMain::Update()
{
	// TODO : ī�޶� ����ٴϰ� ����

	if (KEYBOARD->KeyPress(VK_UP))
		CAMERA->MoveForward();
	else if (KEYBOARD->KeyPress(VK_DOWN))
		CAMERA->MoveBackward();

	if (KEYBOARD->KeyPress(VK_LEFT))
		CAMERA->MoveLeft();
	else if (KEYBOARD->KeyPress(VK_RIGHT))
		CAMERA->MoveRight();

	if (KEYBOARD->KeyPress('E'))
		CAMERA->MoveUp();
	else if (KEYBOARD->KeyPress('Q'))
		CAMERA->MoveDown();

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
				if (rotationAngle < 180.0f) rotationAngle += 6.0f;
				else if (rotationAngle > 180.0f) rotationAngle -= 6.0f;
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

		// ȸ�� �� D3DXMatrixRotationAxis�� ����ؼ�
		// ��ȯ���(matRotation), ȸ����(Y��), ȸ����(rotationAngle)�� �ִ´�.


		D3DXMATRIX matScale, matTranslation, matRotation;
		D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f);
		//rotationAngle = rotationAngle * Frames::Get()->TimeElapsed();
		//D3DXMatrixRotationY(&matRotation, rotationAngle);
		D3DXMatrixRotationAxis(&matRotation, &D3DXVECTOR3(0, 1, 0) , rotationAngle * 2 * Frames::Get()->TimeElapsed());
		//D3DXMatrixRotationYawPitchRoll(&matRotation, rotationAngle, 0, 0);
		D3DXMatrixTranslation(&matTranslation, location.x, location.y, location.z);

		world = matScale * matRotation * matTranslation;

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

	if (MOUSE->ButtonPress(1))
	{
		D3DXVECTOR3 move = MOUSE->GetMoveValue();

		CAMERA->Rotate(D3DXVECTOR2(move.y, move.x));
	}

	skydome->Update();
	landscape->Update();

	model->Update();

	CAMERA->Update();
}

void GameMain::PreRender()
{
	renderTexture->SetTarget();
	renderTexture->Clear();
	{

	}
	DepthStencil::Get()->SetDefaultRenderView();
}

void GameMain::Render()
{
	skydome->Render();
	landscape->Render();

	//Rasterizer::Get()->SetWireframe();
	model->Render();
	//Rasterizer::Get()->SetSolid();
}
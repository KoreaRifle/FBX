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

	isMoving = isAttack = isTrigger = false;

	model = new ModelScene();

	model->SetRootFilePath(filePath);

	model->LoadScene(tPosePath, true, true, true, false);
	model->LoadScene(idlePath, false, false, false, true);
	model->LoadScene(runPath, false, false, false, true);
	model->LoadScene(attackPath, false, false, false, true);

	// Default Animation 설정
	model->SetCurrentAnimation(idlePath);

	D3DXMatrixIdentity(&world);

	location = D3DXVECTOR3(0, 0, 0);
	rotation = D3DXVECTOR3(0, 0, 0);

	UserInterface::Get()->AddFbxModelTransloate(&location.x, &location.y, &location.z);

	// TODO : 캐릭터 회전 필요
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

	// W 누르면 캐릭터가 앞으로 움직이는 애니메이션 동작
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
				location.z++;
			if (KEYBOARD->KeyPress('S'))
				location.z--;
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
				location.x++;
			if (KEYBOARD->KeyPress('A'))
				location.x--;
		}
		// 키 입력 중지 상태
		if (KEYBOARD->KeyUp('W') || KEYBOARD->KeyUp('S') || KEYBOARD->KeyUp('A') || KEYBOARD->KeyUp('D'))
		{
			isMoving = false;
			isTrigger = false;
			model->SetCurrentAnimation(idlePath);
		}

		D3DXMatrixTranslation(&world, location.x, location.y, location.z);

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
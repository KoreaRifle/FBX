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

	//model = new ModelScene(L"./Contents/Models/Box.fbx");
	//model = new ModelScene(L"./Contents/Models/Rock_01.fbx");

	tPosePath = filePath + L"paladin_prop_j_nordstrom.fbx";
	idlePath  = filePath + L"sword and shield idle.fbx";
	runPath = filePath + L"sword and shield run.fbx";
	attackPath = filePath + L"sword and shield slash.fbx";

	isMoving = isAttack = isTrigger = false;

	model = new ModelScene();

	model->LoadScene(tPosePath, true, true, true, false);
	model->LoadScene(idlePath, false, false, false, true);
	model->LoadScene(runPath, false, false, false, true);
	model->LoadScene(attackPath, false, false, false, true);

	// Default Animation 설정
	model->SetCurrentAnimation(idlePath);

	D3DXMatrixIdentity(&world);

	tempX = 0.0f;
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
	if (Keyboard::Get()->KeyPress(VK_UP))
		Camera::Get()->MoveForward();
	else if (Keyboard::Get()->KeyPress(VK_DOWN))
		Camera::Get()->MoveBackward();

	if (Keyboard::Get()->KeyPress(VK_LEFT))
		Camera::Get()->MoveLeft();
	else if (Keyboard::Get()->KeyPress(VK_RIGHT))
		Camera::Get()->MoveRight();

	if (Keyboard::Get()->KeyPress('E'))
		Camera::Get()->MoveUp();
	else if (Keyboard::Get()->KeyPress('Q'))
		Camera::Get()->MoveDown();

	// W 누르면 캐릭터가 앞으로 움직이는 애니메이션 동작
	if (isAttack == false)
	{
		if (Keyboard::Get()->KeyPress('W') || Keyboard::Get()->KeyPress('S'))
		{
			isMoving = true;
			if (isTrigger == false)
			{
				isTrigger = true;
				model->SetCurrentAnimation(runPath);
			}

			D3DXMatrixTranslation(&world, tempX, 0.0f, 0.0f);
			
			model->SetWorldTransform(world);
		}
		else if (Keyboard::Get()->KeyUp('W') || Keyboard::Get()->KeyUp('S'))
		{
			isMoving = false;
			isTrigger = false;
			model->SetCurrentAnimation(idlePath);
		}
	}

	tempX++;
	

	// 마우스 좌클릭 시 공격하는 애니메이션 동작
	if (isMoving == false)
	{
		if (Mouse::Get()->ButtonDown(0))
		{
			isAttack = true;
			model->SetCurrentAnimation(attackPath);
		}
		else if (Mouse::Get()->ButtonUp(0))
		{
			isAttack = false;
			model->SetCurrentAnimation(idlePath);
		}
	}

	if (Mouse::Get()->ButtonPress(1))
	{
		D3DXVECTOR3 move = Mouse::Get()->GetMoveValue();

		Camera::Get()->Rotate(D3DXVECTOR2(move.y, move.x));
	}

	skydome->Update();
	landscape->Update();

	model->Update();

	Camera::Get()->Update();
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
	//landscape->Render();

	//Rasterizer::Get()->SetWireframe();
	model->Render();
	//Rasterizer::Get()->SetSolid();
}
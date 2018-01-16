#include "stdafx.h"
#include "GameMain.h"
#include "./Render/RenderTexture.h"
#include "./Render/D2DRender.h"
#include "./Landscape/Landscape.h"
#include "./Landscape/Skydome.h"
#include "./Character/CharacterManager.h"

void GameMain::Initialize()
{
	UserInterface::Get();
	Shader::CreateBuffer();
	
	renderTexture = new RenderTexture();
	skydome = new Skydome();
	landscape = new Landscape();

	//TODO 나중에 Player와 Enemy 클래스를 Character 클래스로 통합할 예정
	characterManager = new CharacterManager();
	characterManager->CreateObject();
}

void GameMain::Destroy()
{
	SAFE_DELETE(characterManager);

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
	else if (Keyboard::Get()->KeyPress(VK_DOWN))
		CAMERA->MoveBackward();

	if (KEYBOARD->KeyPress(VK_LEFT))
		CAMERA->MoveLeft();
	else if (Keyboard::Get()->KeyPress(VK_RIGHT))
		CAMERA->MoveRight();

	if (MOUSE->ButtonPress(1))
	{
		D3DXVECTOR3 move = MOUSE->GetMoveValue();

		CAMERA->Rotate(D3DXVECTOR2(move.y, move.x));
	}

	skydome->Update();
	landscape->Update();
	characterManager->Update();

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

	characterManager->Render();

	//Rasterizer::Get()->SetWireframe();
	//Rasterizer::Get()->SetSolid();
}
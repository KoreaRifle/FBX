#include "stdafx.h"
#include "GameMain.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	srand((UINT)time(NULL));

	D3DInfo info;
	info.appName = L"DirectX Game";
	info.instance = hInstance;
	info.isFullScreen = false;
	info.isVsync = true;
	info.mainHandle = NULL;
	info.screenWidth = 1280;
	info.screenHeight = 720;
	info.clearColor = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	D3D::SetInfo(info);


	GameMain* gameMain = new GameMain();
	gameMain->Run();
	delete gameMain;

	return 0;
}
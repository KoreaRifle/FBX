#pragma once
#include "./System/Window.h"

class RenderTexture;
class Skydome;
class Landscape;
class Player;
//class Enemy;
class GameMain : public Window
{
public:
	void Initialize();
	void Destroy();
	void Update();
	void PreRender();
	void Render();

private:
	RenderTexture* renderTexture;

	Skydome* skydome;
	Landscape* landscape;
	Player* player;
	//Enemy* enemyMutant;
};
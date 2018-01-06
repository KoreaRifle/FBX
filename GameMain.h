#pragma once
#include "./System/Window.h"

class RenderTexture;
class Skydome;
class Landscape;

class ModelScene;
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

	ModelScene* model;

private:
	wstring tPosePath;
	wstring idlePath;
	wstring runPath;
	wstring attackPath;
	wstring turnPath;

	bool isMoving;
	bool isAttack;
	bool isTrigger;

	D3DXMATRIX world;

	float scale;
	D3DXVECTOR3 location;
	//D3DXVECTOR3 rotation;
	//D3DXVECTOR3 rotationNormalize;
	float rotationAngle;
	float playerDirection;
};
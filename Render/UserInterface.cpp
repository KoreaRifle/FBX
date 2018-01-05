#include "../stdafx.h"
#include "UserInterface.h"

UserInterface* UserInterface::instance = NULL;
TwBar* UserInterface::bar = NULL;

UserInterface * UserInterface::Get()
{
	if (instance == NULL)
		instance = new UserInterface();

	return instance;
}

void UserInterface::Delete()
{
	SAFE_DELETE(instance);
}

void UserInterface::AddCamera(D3DXVECTOR3 * position)
{
	TwAddVarRW(bar, "Position_X", TW_TYPE_FLOAT, &position->x, "group='CameraPosition'");
	TwAddVarRW(bar, "Position_Y", TW_TYPE_FLOAT, &position->y, "group='CameraPosition'");
	TwAddVarRW(bar, "Position_Z", TW_TYPE_FLOAT, &position->z, "group='CameraPosition'");
}

void UserInterface::AddSun(D3DXVECTOR3 * lightDir)
{
	TwAddVarRW(bar, "Light Direction", TW_TYPE_DIR3F, lightDir, "group='sun'");
}

void UserInterface::AddMaterial(string groupName, D3DXCOLOR * ambient, D3DXCOLOR * diffuse)
{
	string option = "group='" + groupName + "'";

	TwAddVarRW(bar, "Ambient", TW_TYPE_COLOR3F, ambient, option.c_str());
	TwAddVarRW(bar, "Diffuse", TW_TYPE_COLOR3F, diffuse, option.c_str());
}

void UserInterface::AddSkydome(D3DXCOLOR * apex, D3DXCOLOR * center)
{
	TwAddVarRW(bar, "Apex", TW_TYPE_COLOR3F, apex, "group='Sky'");
	TwAddVarRW(bar, "Center", TW_TYPE_COLOR3F, center, "ground='Sky'");
}

void UserInterface::AddFbxModelScale(float * scale)
{
	TwAddVarRW(bar, "Scale", TW_TYPE_FLOAT, scale, "group='FbxModelScale' min=0.1 max=1.0 step=0.1");
}

UserInterface::UserInterface()
	: fillMode(0)
{
	bar = TweakBar::Get()->GetBar();

	function<void(void *)> f = bind
	(
		&UserInterface::FillMode
		, this
		, placeholders::_1
	);
	TweakBar::Add(&fillMode, f);

	TwAddButton(bar, "Fill Mode", Notified, &fillMode, "");
}

UserInterface::~UserInterface()
{
}

void UserInterface::FillMode(void * value)
{
	UINT* mode = (UINT *)value;

	if (*mode == 0)
	{
		*mode = 1;
		Rasterizer::Get()->SetWireframe();
	}
	else if(*mode == 1)
	{
		*mode = 0;
		Rasterizer::Get()->SetSolid();
	}
}

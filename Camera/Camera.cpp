#include "../stdafx.h"
#include "Camera.h"

Camera* Camera::instance = NULL;

const float Camera::screenNear = 0.1f;
const float Camera::screenDepth = 1000.0f;

Camera::Camera()
	: position(0, 0, 0)
	, forward(0, 0, 0), right(0, 0, 0), up(0, 0, 0)
	, rotate(0, 0)
	, translationSpeed(100.0f), rotationSpeed(2.5f)
	, playerCamLocation(0, 0, 0)
{
	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&projection);

	UpdateOrthoMatrix();
	UpdateProjectionMatrix();
	UpdateRotationMatrix();
	UpdateViewMatrix();

	//UserInterface::AddCamera(&position);
	UserInterface::AddCamera(&playerCamLocation);
}

Camera::~Camera()
{
}

Camera * Camera::Get()
{
	if (instance == NULL)
		instance = new Camera();

	return instance;
}

void Camera::Delete()
{
	SAFE_DELETE(instance);
}

void Camera::UpdateRotationMatrix()
{
	D3DXMATRIX x, y;
	D3DXMatrixRotationX(&x, rotate.x);
	D3DXMatrixRotationY(&y, rotate.y);
	
	rotation = x * y;

	D3DXVec3TransformCoord(&forward, &D3DXVECTOR3(0, 0, 1), &rotation);
	D3DXVec3TransformCoord(&right, &D3DXVECTOR3(1, 0, 0), &rotation);
	D3DXVec3TransformCoord(&up, &D3DXVECTOR3(0, 1, 0), &rotation);
}

void Camera::UpdateOrthoMatrix()
{
	D3DInfo info;
	D3D::GetInfo(&info);

	D3DXMatrixOrthoOffCenterLH
	(
		&ortho
		, 0, (float)info.screenWidth
		, 0, (float)info.screenHeight
		, 0, 1
	);
}

void Camera::UpdateProjectionMatrix()
{
	D3DInfo info;
	D3D::GetInfo(&info);

	viewport.Width = (float)info.screenWidth;
	viewport.Height = (float)info.screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	D3D::GetDeviceContext()->RSSetViewports(1, &viewport);

	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)info.screenWidth / (float)info.screenHeight;

	D3DXMatrixPerspectiveFovLH(&projection, fieldOfView, screenAspect, screenNear, screenDepth);
}

void Camera::UpdateViewMatrix()
{
	//D3DXMatrixLookAtLH(&view, &position, &(position + forward), &up);
	D3DXMatrixLookAtLH(&view, &playerCamLocation, &(playerCamLocation + forward), &up);
}

void Camera::Update()
{
	UpdateViewMatrix();
}

void Camera::Move(D3DXVECTOR3 translation)
{
	//position += translation * Frames::TimeElapsed();
	playerCamLocation += translation * Frames::TimeElapsed();
	UpdateViewMatrix();
}

void Camera::MoveUp()
{
	Move(up * translationSpeed);
}

void Camera::MoveDown()
{
	Move(-up * translationSpeed);
}

void Camera::MoveLeft()
{
	Move(-right * translationSpeed);
}

void Camera::MoveRight()
{
	Move(right * translationSpeed);
}

void Camera::MoveForward()
{
	Move(forward * translationSpeed);
}

void Camera::MoveBackward()
{
	Move(-forward * translationSpeed);
}

void Camera::Rotate(D3DXVECTOR2 rotate)
{
	this->rotate.x += rotate.x * Frames::TimeElapsed() * rotationSpeed;
	this->rotate.y += rotate.y * Frames::TimeElapsed() * rotationSpeed;

	UpdateRotationMatrix();
	UpdateViewMatrix();
}

void Camera::RotateUp()
{
	Rotate(D3DXVECTOR2(1, 0));
}

void Camera::RotateDown()
{
	Rotate(D3DXVECTOR2(-1, 0));
}

void Camera::RotateLeft()
{
	Rotate(D3DXVECTOR2(0, 1));
}

void Camera::RotateRight()
{
	Rotate(D3DXVECTOR2(0, -1));
}

void Camera::GetRay(D3DXVECTOR3 * origin, D3DXVECTOR3 * direction)
{
	D3DXVECTOR3 mouse = Mouse::Get()->GetPosition();

	D3DInfo info;
	D3D::GetInfo(&info);


	D3DXVECTOR2 point;
	point.x = ((2.0f * mouse.x) / info.screenWidth) - 1.0f;
	point.y = (((2.0f * mouse.y) / info.screenHeight) - 1.0f) * -1.0f;

	point.x = point.x / projection._11;
	point.y = point.y / projection._22;

	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);

	D3DXVECTOR3 dir;
	dir.x = (point.x * invView._11) + (point.y * invView._21) + invView._31;
	dir.y = (point.x * invView._12) + (point.y * invView._22) + invView._32;
	dir.z = (point.x * invView._13) + (point.y * invView._23) + invView._33;

	D3DXVec3Normalize(&dir, &dir);
	
	//*origin = position;
	*origin = playerCamLocation;
	*direction = dir;
}

// �÷��̾ ����ٴϴ� ī�޶� ����
void Camera::SetPlayerLocation(D3DXVECTOR3 location)
{
	D3DXVECTOR3 playerLocation = location;

	//6.0f, 18.0f, -44.0f
	playerCamLocation.x = location.x + 6.0f;
	playerCamLocation.y = location.y + 18.0f;
	playerCamLocation.z = location.z - 35.0f;
}

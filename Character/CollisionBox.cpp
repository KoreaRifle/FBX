#include "../stdafx.h"
#include "CollisionBox.h"

//TODO 캐릭터 크기만큼 콜리전 박스를 생성해야함
CollisionBox::CollisionBox(D3DXVECTOR3 minValue, D3DXVECTOR3 maxValue)
	: Shader(L"./Character/Character.fx")
{
	forceScale = 0.1f;
	//UserInterface::Get()->GetFbxModelScale(&forceScale);

	minCoord = minValue;
	maxCoord = maxValue;
	
	centerCircle = minCoord + ((maxCoord - minCoord) * 0.5f);
	
	float x = maxCoord.x - minCoord.x;
	float y = maxCoord.y - minCoord.y;
	float z = maxCoord.z - minCoord.z;

	// hypotenuse를 구하기 위한 삼각형의 width, height 계산
	// tempWidth는 x,y 좌표를 피타고라스 식을 통해 계산
	float tempWidth = sqrt((x * x) + (y * y));
	float tempHeight = y;

	// hypotenuse : minCoord와 maxCoord 사이의 거리값
	hypotenuse = sqrt((tempWidth * tempWidth) + (y * y));

	CreateBuffer();
	CreateInputLayout(VertexColor::desc, VertexColor::count);

	D3DXMatrixIdentity(&world);
}

CollisionBox::~CollisionBox()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void CollisionBox::Update()
{
	worldBuffer->SetWorld(world);

	Shader::Update();
}

void CollisionBox::Render()
{
	ID3D11DeviceContext* dc = D3D::GetDeviceContext();

	UINT stride = sizeof(VertexColor);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	Shader::Render();

	dc->DrawIndexed(indexCount, 0, 0);
}

void CollisionBox::CreateBuffer()
{
	radius = hypotenuse * 0.5f * forceScale;

	HRESULT hr;

	UINT* index = NULL;

	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	UINT axisCount = 36;
	UINT axisX, axisY, axisZ;
	axisX = axisCount;
	axisY = axisX * 2;
	axisZ = axisX * 3;
	vertexCount = axisCount * 3;
	indexCount = vertexCount * 2;

	vertex = new VertexColor[vertexCount];

	float prevRadian, curRadian;
	prevRadian = curRadian = 0.0f;

	for (UINT i = 0; i < vertexCount; i++)
	{
		// x축 회전
		if (i < axisX)
		{
			curRadian = prevRadian + (2.0f * (float)D3DX_PI / axisCount);
			vertex[i].position.x = centerCircle.x * forceScale;
			vertex[i].position.y = sinf(curRadian) * radius;
			vertex[i].position.z = cosf(curRadian) * radius;
			prevRadian = curRadian;
		}

		// y축 회전
		if (i == axisX) prevRadian = curRadian = 0.0f;
		if (i >= axisX && i < axisY)
		{
			curRadian = prevRadian + (2.0f * (float)D3DX_PI / axisCount);
			vertex[i].position.x = cosf(curRadian) * radius;
			vertex[i].position.y = centerCircle.y * forceScale;
			vertex[i].position.z = sinf(curRadian) * radius;
			prevRadian = curRadian;
		}

		// z축 회전
		if (i == axisY) prevRadian = curRadian = 0.0f;
		if (i >= axisY && i < axisZ)
		{
			curRadian = prevRadian + (2.0f * (float)D3DX_PI / axisCount);
			vertex[i].position.x = cosf(curRadian) * radius;
			vertex[i].position.y = sinf(curRadian) * radius;
			vertex[i].position.z = centerCircle.z * forceScale;
			prevRadian = curRadian;
		}
		
		vertex[i].color = 0xFFFF0000;
	}

	index = new UINT[indexCount];
	for (UINT i = 0; i < vertexCount; i++)
	{
		// 0,1,1,2,2,3,3,4....
		if (i < axisX)
		{
			index[2 * i] = i;
			if (i != axisX - 1) index[2 * i + 1] = i + 1;
			else index[2 * i + 1] = 0;
		}
		else if (i >= axisX && i < axisY)
		{
			index[2 * i] = i;
			if (i != axisY - 1) index[2 * i + 1] = i + 1;
			else index[2 * i + 1] = axisX;
		}
		else if (i >= axisY && i < axisZ)
		{
			index[2 * i] = i;
			//index[2 * i + 1] = i + 1;
			if (i != axisZ - 1) index[2 * i + 1] = i + 1;
			else index[2 * i + 1] = axisY;
		}
	}

	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexColor) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertex;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	//2. Index Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = index;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));

	SAFE_DELETE_ARRAY(index);
	SAFE_DELETE_ARRAY(vertex);
}

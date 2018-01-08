#include "../stdafx.h"
#include "CollisionBox.h"

#define ForceScale 0.1f

CollisionBox::CollisionBox(D3DXVECTOR3 minValue, D3DXVECTOR3 maxValue)
	: Shader(L"./Character/Character.fx")
{
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
	float hypotenuse = sqrt((tempWidth * tempWidth) + (y * y));

	// TODO : UserInterface의 scale 조정값을 가지고 와야함(현재 min/max크기의 0.1f 곱한값 강제 할당)
	radius = hypotenuse * 0.5f * ForceScale;
	
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
	HRESULT hr;

	UINT* index = NULL;

	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	vertexCount = 360;
	indexCount = vertexCount * 2;

	vertex = new VertexColor[vertexCount];

	float prevRadian, curRadian;
	prevRadian = curRadian = 0.0f;

	for (int i = 0; i < 360; i++)
	{
		curRadian = prevRadian + ((float)D3DX_PI / 180.0f);
		vertex[i].position.x = cosf(curRadian) * radius;
		vertex[i].position.y = centerCircle.y * ForceScale;
		vertex[i].position.z = sinf(curRadian) * radius;
		prevRadian = curRadian;

		vertex[i].color = 0xFFFF0000;
	}

	// TODO : 인덱스 설정했음에도 반원만 그려짐
	index = new UINT[indexCount];
	for (UINT i = 0; i < vertexCount; i++)
	{
		// 0,1,1,2,2,3,3,4....
		index[2 * i] = i;
		index[2 * i + 1] = i + 1;
	}
	index[vertexCount - 1] = 0; // 반원 확인용(최종 빌드 시 지워야함)

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
	desc.ByteWidth = sizeof(UINT) * vertexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = index;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));

	SAFE_DELETE_ARRAY(vertex);
	SAFE_DELETE_ARRAY(index);
}

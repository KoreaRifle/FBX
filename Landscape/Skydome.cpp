#include "../stdafx.h"
#include "Skydome.h"
#include "SkydomeBuffer.h"

Skydome::Skydome()
	: Shader(L"./Landscape/Skydome.fx")
	, meshFile(L"./Contents/Meshes/Sphere.data")
{
	CreateBuffer();
	CreateInputLayout(Vertex::desc, Vertex::count);

	skydomebuffer = new SkydomeBuffer();
}

Skydome::~Skydome()
{
	SAFE_DELETE(skydomebuffer);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void Skydome::Update()
{
	D3DXVECTOR3 position;
	Camera::Get()->GetPosition(&position);

	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	
	worldBuffer->SetWorld(world);
	skydomebuffer->Update();

	Shader::Update();
}

void Skydome::Render()
{
	ID3D11DeviceContext* dc = D3D::GetDeviceContext();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	skydomebuffer->SetPSBuffer(1);

	Shader::Render();

	DepthStencil::Get()->SetOffState();
	Rasterizer::Get()->SetOffCullMode();
	{
		dc->DrawIndexed(indexCount, 0, 0);
	}
	DepthStencil::Get()->SetOnState();
	Rasterizer::Get()->SetOnCullMode();
}

void Skydome::CreateBuffer()
{
	HRESULT hr;

	Mesh mesh;
	mesh.Open(meshFile);
	
	Vertex* vertex = NULL;
	mesh.CopyVertex(&vertex, &vertexCount);

	UINT* index = NULL;
	mesh.CopyIndex(&index, &indexCount);


	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	
	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(Vertex) * vertexCount;
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

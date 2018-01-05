#include "../stdafx.h"
#include "D2DRender.h"

D2DRender::D2DRender(ID3D11ShaderResourceView* resourceView)
	: Shader(L"./Render/D2DRender.fx")
	, resourceView(resourceView)
	, translation(0, 0), scale(1, 1)
{
	CreateInputLayout(VertexTexture::desc, VertexTexture::count);

	VertexTexture* vertex = new VertexTexture[4];
	vertex[0].position = D3DXVECTOR3(0, 0, 0);
	vertex[0].uv = D3DXVECTOR2(0, 1);

	vertex[1].position = D3DXVECTOR3(0, 1, 0);
	vertex[1].uv = D3DXVECTOR2(0, 0);

	vertex[2].position = D3DXVECTOR3(1, 0, 0);
	vertex[2].uv = D3DXVECTOR2(1, 1);

	vertex[3].position = D3DXVECTOR3(1, 1, 0);
	vertex[3].uv = D3DXVECTOR2(1, 0);

	UINT* index = new UINT[6]{ 0, 1, 2, 2, 1, 3 };


	HRESULT hr;
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexTexture) * 4;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertex;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	//2. Index Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * 6;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = index;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));

	SAFE_DELETE_ARRAY(vertex);
	SAFE_DELETE_ARRAY(index);

	
D3DXVECTOR2 half = D3DXVECTOR2(0.5f, 0.5f);
	D3DXMatrixTranslation(&center, half.x, half.y, 0);
	D3DXMatrixInverse(&invCenter, NULL, &center);

	SetScale(scale);
	SetTranslation(translation);

	D3DXMatrixIdentity(&view);
}

D2DRender::~D2DRender()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void D2DRender::Update()
{
	world = S * T;
	worldBuffer->SetWorld(world);
	worldBuffer->Update();

	
	D3DXMATRIX ortho;
	Camera::Get()->GetOrtho(&ortho);

	cameraBuffer->SetUserMatrix();
	cameraBuffer->SetMatrix(view, ortho);
	cameraBuffer->Update();

	Shader::Update();
}

void D2DRender::Render()
{
	ID3D11DeviceContext* dc = D3D::GetDeviceContext();

	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dc->PSSetShaderResources(0, 1, &resourceView);

	Shader::Render();

	DepthStencil::Get()->SetOffState();
	{
		dc->DrawIndexed(6, 0, 0);
	}
	DepthStencil::Get()->SetOffState();
}

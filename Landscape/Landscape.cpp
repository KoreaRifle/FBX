#include "../stdafx.h"
#include "Landscape.h"
#include "../Shader/MaterialBuffer.h"

Landscape::Landscape()
	: Shader(L"./Landscape/Landscape.fx")
	, diffuseMapFile(L"./Contents/Textures/White.png")
	, width(100), height(100)
{
	HRESULT hr;	

	CreateVertexData();
	CreateIndexData();
	CreateBuffer();

	CreateInputLayout(VertexTextureNormal::desc, VertexTextureNormal::count);

	hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice()
		, diffuseMapFile.c_str()
		, NULL
		, NULL
		, &diffuseMap
		, NULL
	);
	assert(SUCCEEDED(hr));

	materialBuffer = new MaterialBuffer("Landscape");
	materialBuffer->SetAmbient(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	materialBuffer->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

Landscape::~Landscape()
{
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(materialBuffer);

	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);
	
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);
}

void Landscape::CreateVertexData()
{
	vertexCount = (width + 1) * (height + 1);

	UINT heightIndex = 0;
	vertexData = new VertexTextureNormal[vertexCount];
	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			int index = (width + 1) * z + x;

			vertexData[index].position.x = (float)x - width / 2;
			vertexData[index].position.y = (float)0.0f;
			vertexData[index].position.z = (float)z - height / 2;

			vertexData[index].uv.x = (x * 10) / (float)width;
			vertexData[index].uv.y = (z * 10) / (float)height;

			vertexData[index].normal = D3DXVECTOR3(0, 0, 0);
		}
	}
}

void Landscape::CreateIndexData()
{
	indexCount = width * height * 6;
	indexData = new UINT[indexCount];

	UINT count = 0;
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			indexData[count + 0] = (width + 1) * z + x;
			indexData[count + 1] = (width + 1) * (z + 1) + x;
			indexData[count + 2] = (width + 1) * z + x + 1;
			indexData[count + 3] = (width + 1) * z + x + 1;
			indexData[count + 4] = (width + 1) * (z + 1) + x;
			indexData[count + 5] = (width + 1) * (z + 1) + (x + 1);
			
			count += 6;
		}//for(x)
	}//for(z)
}

void Landscape::CreateBuffer()
{
	HRESULT hr;

	D3D11_BUFFER_DESC vertexDesc = { 0 };
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.ByteWidth = sizeof(VertexTextureNormal) * vertexCount;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResource = { 0 };
	vertexSubResource.pSysMem = vertexData;

	hr = D3D::GetDevice()->CreateBuffer(&vertexDesc, &vertexSubResource, &vertexBuffer);
	assert(SUCCEEDED(hr));


	D3D11_BUFFER_DESC indexDesc = { 0 };
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.ByteWidth = sizeof(UINT) * indexCount;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubResource = { 0 };
	indexSubResource.pSysMem = indexData;

	hr = D3D::GetDevice()->CreateBuffer(&indexDesc, &indexSubResource, &indexBuffer);
	assert(SUCCEEDED(hr));
}

void Landscape::Update()
{
	materialBuffer->Update();

	Shader::Update();
}

void Landscape::Render()
{
	UINT stride = sizeof(VertexTextureNormal);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &diffuseMap);

	Shader::Render();
	materialBuffer->SetPSBuffer(1);

	Sampler::Get()->SetDefault();
	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);	
}

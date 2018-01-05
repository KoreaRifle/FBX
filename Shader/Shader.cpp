#include "../stdafx.h"
#include "Shader.h"

SunBuffer* Shader::sunBuffer = NULL;

void Shader::Update()
{
	cameraBuffer->Update();
	worldBuffer->Update();

	sunBuffer->Update();
}

void Shader::Render()
{
	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	cameraBuffer->SetVSBuffer(0);
	worldBuffer->SetVSBuffer(1);
	sunBuffer->SetPSBuffer(0);
}

void Shader::CreateBuffer()
{
	if (sunBuffer == NULL)
		sunBuffer = new SunBuffer();
}

void Shader::DeleteBuffer()
{
	SAFE_DELETE(sunBuffer);
}

Shader::Shader(wstring shaderFile)
	: shaderFile(shaderFile)
{
	CreateVertexShader();
	CreatePixelShader();

	worldBuffer = new WorldBuffer();
	cameraBuffer = new CameraBuffer();
}

Shader::~Shader()
{
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(cameraBuffer);

	SAFE_RELEASE(layout);

	SAFE_RELEASE(vertexBlob);
	SAFE_RELEASE(vertexShader);

	SAFE_RELEASE(pixelBlob);
	SAFE_RELEASE(pixelShader);
}

void Shader::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC * desc, UINT count)
{
	HRESULT hr = D3D::GetDevice()->CreateInputLayout
	(
		desc
		, count
		, vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, &layout
	);
	assert(SUCCEEDED(hr));
}

void Shader::CreateVertexShader()
{
	ID3D10Blob* error;
	HRESULT hr = D3DX10CompileFromFile
	(
		shaderFile.c_str(), NULL, NULL, "VS", "vs_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &vertexBlob, &error, NULL
	);
	CheckShaderError(hr, error);

	hr = D3D::GetDevice()->CreateVertexShader
	(
		vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, NULL
		, &vertexShader
	);
	assert(SUCCEEDED(hr));
}

void Shader::CreatePixelShader()
{
	ID3D10Blob* error;
	HRESULT hr = D3DX10CompileFromFile
	(
		shaderFile.c_str(), NULL, NULL, "PS", "ps_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &pixelBlob, &error, NULL
	);
	CheckShaderError(hr, error);
	
	hr = D3D::GetDevice()->CreatePixelShader
	(
		pixelBlob->GetBufferPointer()
		, pixelBlob->GetBufferSize()
		, NULL
		, &pixelShader
	);
	assert(SUCCEEDED(hr));
}

void Shader::CheckShaderError(HRESULT hr, ID3D10Blob * error)
{
	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char *)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}
}
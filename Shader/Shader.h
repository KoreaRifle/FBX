#pragma once

#include "CameraBuffer.h"
#include "SunBuffer.h"
#include "WorldBuffer.h"

class Shader
{
public:
	static void CreateBuffer();
	static void DeleteBuffer();

protected:
	Shader(wstring shaderFile);
	~Shader();

	void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT count);

	virtual void Update();
	virtual void Render();

	ID3D10Blob* vertexBlob;
	ID3D10Blob* pixelBlob;

	WorldBuffer* worldBuffer;
	CameraBuffer* cameraBuffer;

private:
	static SunBuffer* sunBuffer;

	void CreateVertexShader();
	void CreatePixelShader();
	void CheckShaderError(HRESULT hr, ID3D10Blob* error);

	wstring shaderFile;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* layout;
};
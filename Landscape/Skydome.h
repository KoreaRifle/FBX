#pragma once
#include "../Shader/Shader.h"

class SkydomeBuffer;
class Skydome : public Shader
{
public:
	Skydome();
	~Skydome();

	void Update();
	void Render();

private:
	void CreateBuffer();

	wstring meshFile;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	SkydomeBuffer* skydomebuffer;

	UINT vertexCount;
	UINT indexCount;
};
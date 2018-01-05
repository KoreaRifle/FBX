#pragma once
#include "../Shader/Shader.h"

class Brush;
class MaterialBuffer;
class Landscape : public Shader
{
public:
	Landscape();
	~Landscape();

	void CreateVertexData();
	void CreateIndexData();
	void CreateBuffer();

	void Update();
	void Render();

private:
	Brush* brush;

	MaterialBuffer* materialBuffer;

	wstring diffuseMapFile;
	ID3D11ShaderResourceView* diffuseMap;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT width, height;
	VertexTextureNormal* vertexData;
	UINT* indexData;

	UINT vertexCount;
	UINT indexCount;
};
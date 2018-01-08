#pragma once
#include "../Shader/Shader.h"

class CollisionBox : public Shader
{
public:
	CollisionBox(D3DXVECTOR3 minValue, D3DXVECTOR3 maxValue);
	~CollisionBox();

	void Update();
	void Render();

	void SetWorldMatrix(D3DXMATRIX& world) { this->world = world; }

private:
	void CreateBuffer();

	ID3D11Buffer* vertexBuffer;
	VertexColor* vertex;

	UINT vertexCount;

	ID3D11Buffer* indexBuffer;
	UINT* index;
	UINT indexCount;

	D3DXMATRIX world;

private:
	D3DXVECTOR3 minCoord, maxCoord;
	D3DXVECTOR3 centerCircle;
	float radius;
};


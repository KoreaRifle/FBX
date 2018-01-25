#pragma once
#include "../Shader/Shader.h"

class CollisionBox : public Shader
{
public:
	//CollisionBox(D3DXVECTOR3 minValue, D3DXVECTOR3 maxValue, float forceScale = 1.0f);
	CollisionBox();
	~CollisionBox();

	void CalcCollider(D3DXVECTOR3 minValue, D3DXVECTOR3 maxValue);

	void Update();
	void Render();

	void SetWorldMatrix(D3DXMATRIX& world) { this->world = world; }
	void SetForceScale(float forceScale) { this->forceScale = forceScale; }

	void SetScale(float scale) {}

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
	float hypotenuse; // ºøº¯
	float radius;

	float forceScale;

	float scale;
};


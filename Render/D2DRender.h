#pragma once
#include "../Shader/Shader.h"

class D2DRender : public Shader
{
public:
	D2DRender(ID3D11ShaderResourceView* resourceView);
	~D2DRender();

	void SetTranslation(D3DXVECTOR2& translation)
	{
		this->translation = translation;

		D3DXMatrixTranslation(&T, translation.x, translation.y, 0);
		world = S * T;
	}

	void SetScale(D3DXVECTOR2& scale)
	{
		this->scale = scale;

		D3DXMatrixScaling(&S, scale.x, scale.y, 1);
		//S = invCenter * S * center;

		world = S * T;
	}

	void Update();
	void Render();

private:
	ID3D11ShaderResourceView* resourceView;

	D3DXVECTOR2 translation;
	D3DXVECTOR2 scale;

	D3DXMATRIX center, invCenter;
	D3DXMATRIX S, T;

	D3DXMATRIX world, view;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};
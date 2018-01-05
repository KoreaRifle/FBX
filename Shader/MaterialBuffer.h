#pragma once
#include "ShaderBuffer.h"

class MaterialBuffer : public ShaderBuffer
{
public:
	MaterialBuffer(string groupName)
		: ShaderBuffer(sizeof(Data))
	{
		UserInterface::AddMaterial(groupName, &data.ambient, &data.diffuse);
	}

	void SetAmbient(D3DXCOLOR& color)
	{
		data.ambient = color;
	}

	void SetDiffuse(D3DXCOLOR& color)
	{
		data.diffuse = color;
	}

	void Update()
	{
		UpdateBuffer(&data, sizeof(Data));
	}

	struct Data
	{
		D3DXCOLOR ambient;
		D3DXCOLOR diffuse;
	};

private:
	Data data;
};
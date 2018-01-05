#pragma once
#include "../Shader/ShaderBuffer.h"

class SkydomeBuffer : public ShaderBuffer
{
public:
	SkydomeBuffer()
		: ShaderBuffer(sizeof(Data))
	{
		data.apex = D3DXCOLOR(0.0f, 0.05f, 0.6f, 1.0f);
		data.center = D3DXCOLOR(0.0f, 0.5f, 0.8f, 1.0f);

		UserInterface::AddSkydome(&data.apex, &data.center);
	}

	void Update()
	{
		UpdateBuffer(&data, sizeof(data));
	}

	struct Data
	{
		D3DXCOLOR center;
		D3DXCOLOR apex;
	};

private:
	Data data;
};
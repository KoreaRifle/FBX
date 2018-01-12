#pragma once
#include "../Shader/ShaderBuffer.h"

class ModelBuffer : public ShaderBuffer
{
public:
	ModelBuffer()
		: ShaderBuffer(sizeof(Data))
	{
		//TODO ModelBuffer에서 크기값을 강제로 줄이지 않고 ModelPart에서 줄일 수 있을까?
		//? ModelBuffer에서 강제조정한 크기에 대한 매트릭스를 받아 Min/Max를 구한다.
		D3DXMatrixIdentity(&data.boneScale);
		boneScale = 0.1f;

		UserInterface::Get()->SetFbxModelScale(boneScale);
		UserInterface::Get()->AddFbxModelScale(&boneScale);
		
		for(int i = 0; i < 100; i++)
			D3DXMatrixIdentity(&data.boneArray[i]);

		D3DXMatrixScaling(&data.boneScale, boneScale, boneScale, boneScale);

		data.isSkinning = false;
	}

	void Update()
	{
		//D3DXMatrixScaling(&data.boneScale, boneScale, boneScale, boneScale);
		UpdateBuffer(&data, sizeof(data));
	}

	void SetSkinning(bool isSkinning)
	{
		data.isSkinning = (UINT)isSkinning;
	}

	void SetBoneArray(D3DXMATRIX* matrix, UINT count)
	{
		memcpy(data.boneArray, matrix, count * sizeof(D3DXMATRIX));

		for(UINT i = 0; i < count; i++)
			D3DXMatrixTranspose(&data.boneArray[i], &data.boneArray[i]);
	}

	//? 쉐이더에서 자료를 받을 수 있을까?
	D3DXMATRIX GetBoneScaleMatrix() { return data.boneScale; }


	struct Data
	{
		D3DXMATRIX boneScale;
		D3DXMATRIX boneArray[100];
		UINT isSkinning;
		D3DXVECTOR3 padding;
	};

private:
	Data data;
	float boneScale;
};
#include "../stdafx.h"
#include "Model.h"
#include "ModelPart.h"
#include "ModelMaterial.h"
#include "ModelBuffer.h"
#include "ModelBoneWeights.h"
#include "ModelAnimation.h"
#include "ModelAnimationKeyFrames.h"
#include "ModelAnimationController.h"

Model::Model(wstring name, ModelBuffer* modelBuffer)
	: name(name), modelBuffer(modelBuffer)
{
	//D3DXMatrixIdentity(&absoluteTransform);
	D3DXMatrixIdentity(&matGeometricOffset);
	D3DXMatrixIdentity(&matAnimationTransform);

	modelBuffer = new ModelBuffer();
	isOffTrigger = false;
}

Model::~Model()
{
	for each(Pair temp in animationKeyFrames)
		SAFE_DELETE(temp.second);

	for (size_t i = 0; i < parts.size(); i++)
		SAFE_DELETE(parts[i]);
}

void Model::Update(bool isAnimation)
{
	if (isOffTrigger == false)
	{
		for each(ModelPart* part in parts)
			part->Update(isAnimation);
	}
}

void Model::Render()
{
	if (isOffTrigger == false)
	{
		for each(ModelPart* part in parts)
			part->Render();
	}
}

void Model::AddAnimationKeyFrames(ModelAnimationKeyFrames * animationKeyFrames)
{
	this->animationKeyFrames.push_back(Pair(animationKeyFrames->GetAnimationName(), animationKeyFrames));
}

ModelAnimationKeyFrames * Model::GetAnimationKeyFrames(wstring animationName)
{
	for each(Pair temp in animationKeyFrames)
	{
		if (temp.first == animationName)
			return temp.second;
	}

	return NULL;
}

void Model::AddVertex(ModelMaterial * material, D3DXVECTOR3 position, D3DXVECTOR3 normal, D3DXVECTOR2 uv, const ModelBoneWeights& boneWeights)
{
	bool isNew = true;

	ModelPart* part = NULL;
	for (size_t i = 0; i < parts.size(); i++)
	{
		part = parts[i];

		if (material == part->GetMaterial())
		{
			part->AddVertex(position, normal, uv, boneWeights);
			isNew = false;
		}
	}
	 
	if (isNew == true)
	{
		part = new ModelPart(this, material, modelBuffer);
		part->AddVertex(position, normal, uv, boneWeights);
		
		parts.push_back(part);
	}
}

void Model::CreateBuffer()
{
	for each(ModelPart* part in parts)
		part->CreateBuffer();
}

void Model::SetWorldTransform(D3DXMATRIX & world)
{
	for each(ModelPart* part in parts)
		part->SetWorldTransform(world);
}

void Model::GetCollisionBoxMinMaxValue(D3DXVECTOR3 * collisionBoxMin, D3DXVECTOR3 * collisionBoxMax)
{
	// 0번째 part의 min값과 max값을 넣어 기준값을 만듬
	D3DXVECTOR3 tempMinValue, tempMaxValue;
	parts[0]->GetCollisionBoxMinMaxValue(&tempMinValue, &tempMaxValue);

	// tempPartMinValue, tempPartMaxValue 를 통해 파츠별 min, max 값을 받고
	// 0번째 값(tempMinValue, tempMaxValue)과 비교한다.
	for (size_t i = 0; i < parts.size(); i++)
	{
		D3DXVECTOR3 tempPartMinValue, tempPartMaxValue;
		parts[i]->GetCollisionBoxMinMaxValue(&tempPartMinValue, &tempPartMaxValue);

		// 최소값 계산
		if (tempPartMinValue.x < tempMinValue.x) tempMinValue.x = tempPartMinValue.x;
		if (tempPartMinValue.y < tempMinValue.y) tempMinValue.y = tempPartMinValue.y;
		if (tempPartMinValue.z < tempMinValue.z) tempMinValue.z = tempPartMinValue.z;

		// 최대값 계산
		if (tempPartMaxValue.x > tempMaxValue.x) tempMaxValue.x = tempPartMaxValue.x;
		if (tempPartMaxValue.y > tempMaxValue.y) tempMaxValue.y = tempPartMaxValue.y;
		if (tempPartMaxValue.z > tempMaxValue.z) tempMaxValue.z = tempPartMaxValue.z;
	}

	*collisionBoxMin = tempMinValue;
	*collisionBoxMax = tempMaxValue;
}

void Model::SetBinaryFile(BinaryWriter * bw)
{
	for each(ModelPart* part in parts)
	{
		part->SetBinaryFile(bw);
	}
}

void Model::SetBinaryFile(BinaryReader * br)
{
	for each(ModelPart* part in parts)
	{
		part->SetBInaryFile(br);
	}
}

void Model::UpdateAnimation(ModelAnimationController * animationController)
{
	if (animationController != NULL)
	{
		ModelAnimation* animation = animationController->GetCurrentAnimation();

		if (animation != NULL)
		{
			ModelAnimationKeyFrames* keyFrames = NULL;
			for each(Pair temp in animationKeyFrames)
			{
				if (temp.first == animation->GetName())
				{
					keyFrames = temp.second;

					break;
				}
			}

			if (keyFrames != NULL)
			{
				int keyFrame = animationController->GetCurrentKeyFrame();				
				matAnimationTransform = keyFrames->GetKeyFrameTransform(keyFrame);
				
				return;
			}
		}
	}

	D3DXMatrixIdentity(&matAnimationTransform);
}

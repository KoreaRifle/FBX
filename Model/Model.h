#pragma once

class ModelPart;
class ModelMaterial;
class ModelBuffer;
class ModelBoneWeights;
class ModelAnimationKeyFrames;
class ModelAnimationController;
class Model
{
public:
	Model(wstring name, ModelBuffer* modelBuffer);
	~Model();

	void Update(bool isAnimation);
	void UpdateAnimation(ModelAnimationController* animationController);
	void Render();
	
	wstring GetName() { return name; }

	void AddAnimationKeyFrames(ModelAnimationKeyFrames* animationKeyFrames);
	ModelAnimationKeyFrames* GetAnimationKeyFrames(wstring animationName);

	void SetGeometricOffset(const D3DXMATRIX& matGeometricOffset) 
	{ 
		this->matGeometricOffset = matGeometricOffset; 
	}
	
	D3DXMATRIX GetGeometricOffset() 
	{ 
		return matGeometricOffset; 
	}

	D3DXMATRIX GetAnimationTransform() 
	{ 
		return matAnimationTransform; 
	}

	void SetAbsoluteTransform(const D3DXMATRIX& matAbsoluteTransform) 
	{ 
		this->matAnimationTransform = matAbsoluteTransform; 
	}

	void AddVertex(ModelMaterial * material, D3DXVECTOR3 position, D3DXVECTOR3 normal, D3DXVECTOR2 uv, const ModelBoneWeights& boneWeights);
	void CreateBuffer();

	void SetWorldTransform(D3DXMATRIX& world);

private:
	wstring name;

	//D3DXMATRIX absoluteTransform;
	D3DXMATRIX matGeometricOffset;
	D3DXMATRIX matAnimationTransform;

	vector<ModelPart *> parts;

	ModelBuffer* modelBuffer;
	
	typedef pair<wstring, ModelAnimationKeyFrames *> Pair;
	vector<Pair> animationKeyFrames;

public:
	void GetCollisionBoxMinMaxValue(D3DXVECTOR3* collisionBoxMin, D3DXVECTOR3* collisionBoxMax);

private:
	D3DXVECTOR3 collisionBoxMin, collisionBoxMax;

// 검 변경 관련
public:
	void SetOffTrigger(bool isOffTrigger) { this->isOffTrigger = isOffTrigger; }

private:
	bool isOffTrigger;

// 바이너리 입출력 관련
public:
	void SetBinaryFile(BinaryWriter* bw);
	void SetBinaryFile(BinaryReader* br);

	void GetBinaryVector(D3DXVECTOR3* vecPos, D3DXVECTOR3* vecNormal, D3DXVECTOR2* vecUv);
};
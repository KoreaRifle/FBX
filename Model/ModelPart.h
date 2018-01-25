#pragma once
#include "../Shader/Shader.h"

class Model;
class ModelMaterial;
class ModelBoneWeights;
class ModelBuffer;
class ModelPart : public Shader
{
public:
	ModelPart(Model* model, ModelMaterial* material, ModelBuffer* modelBuffer);
	~ModelPart();

	void Update(bool isAnimation);
	void Render();

	void AddVertex(D3DXVECTOR3& position, D3DXVECTOR3& normal, D3DXVECTOR2& uv, const ModelBoneWeights& boneWeights);
	void CreateBuffer();

	ModelMaterial* GetMaterial()
	{
		return material;
	}

	void SetWorldTransform(D3DXMATRIX& world);

private:
	Model* model;

	vector<D3DXVECTOR3> positions;
	vector<D3DXVECTOR3> normals;
	vector<D3DXVECTOR2> uvs;
	vector<UINT> indices;
	vector<ModelBoneWeights> boneWeights;

	ID3D11Buffer* vertexBuffer;
	VertexTextureNormalBlend* vertex;
	UINT vertexCount;

	ID3D11Buffer* indexBuffer;
	UINT* index;
	UINT indexCount;

	ModelBuffer* modelBuffer;
	ModelMaterial* material;

	bool isSkinnedModel;


public:
	void GetCollisionBoxMinMaxValue(D3DXVECTOR3* collisionBoxMin, D3DXVECTOR3* collisionBoxMax);

private:
	D3DXMATRIX world;

	D3DXVECTOR3 collisionBoxMin, collisionBoxMax;

// 바이너리 입출력 관련
public:
	void SetBinaryFile(BinaryWriter* bw);
	void SetBInaryFile(BinaryReader* br);
	void WriteBinaryFile();
	void ReadBinaryFile();

	void GetBinaryVector(D3DXVECTOR3* vecPos, D3DXVECTOR3* vecNormal, D3DXVECTOR2* vecUv);

private:
	BinaryWriter* bw;
	BinaryReader* br;

	static UINT count;
};
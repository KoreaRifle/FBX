#pragma once

class Model;
class ModelMaterial;
class ModelBuffer;
class ModelSkeleton;
class ModelAnimationController;
class ModelBoneWeights;
class ModelScene
{
public:
	ModelScene();
	~ModelScene();

	void LoadScene(wstring file, bool isMaterial, bool isSkeleton, bool isMesh, bool isAnimation, bool isRootBoneLock = false);
	void SetCurrentAnimation(wstring filePath);

	void SetWorldTransform(D3DXMATRIX& world);

	void Update();
	void Render();

	static wstring TexturePath;

private:
	void ProcessScene(bool isMaterial, bool isSkeleton, bool isMesh, bool isAnimation, bool isRootBoneLock);
	void ProcessMaterial();
	void ProcessNode(FbxNode* node, FbxNodeAttribute::EType attribute);
	void ProcessMesh(FbxNode* node);
	void ProcessAnimation(FbxNode* node, wstring takeName, float frameRate, float start, float stop);
	void ProcessAnimations(wstring file);
	void ProcessSkeleton(FbxNode* node);

	void ProcessBoneWeights(FbxMesh* mesh, vector<ModelBoneWeights>& meshBoneWeights);
	void ProcessBoneWeights(FbxSkin* skin, vector<ModelBoneWeights>& meshBoneWeights);

	


	ModelMaterial* LinkMaterialWithPolygon
	(
		FbxMesh* mesh, int layerIndex
		, int polygonIndex, int polygonVertexIndex
		, int vertexIndex
	);

	int GetMappingIndex
	(
		FbxLayerElement::EMappingMode mode
		, int polygonIndex
		, int polygonVertexIndex
		, int vertexIndex
	);

	D3DXVECTOR2 GetUV
	(
		FbxMesh* mesh, int layerIndex
		, int polygonIndex, int polygonVertexIndex
		, int vertexIndex
	);

	ModelMaterial* GetMaterial(FbxSurfaceMaterial* fbxMaterial);
	D3DXMATRIX GetGeometricOffset(FbxNode* node);
	D3DXMATRIX GetAbsoluteTransformFromCurrentTake(FbxNode* node, FbxTime time);


	FbxManager* manager;
	FbxScene* scene;
	FbxGeometryConverter* converter;
	FbxImporter* importer;
	FbxIOSettings* ios;

	ModelSkeleton* skeleton;

	vector<Model *> models;
	vector<ModelMaterial *> materials;
	vector<FbxSurfaceMaterial *> fbxMaterials;

	ModelBuffer* modelBuffer;
	ModelAnimationController* animationController;

	wstring animationFileName;


public:
	void SetRootFilePath(wstring rootFilePath) { this->rootFilePath = rootFilePath; }
	void SetRootBoneLock(bool isRootBoneLock) { this->isRootBoneLock = isRootBoneLock; }
	void GetCollisionBoxMinMaxValue(D3DXVECTOR3* collisionBoxMin, D3DXVECTOR3* collisionBoxMax);
	bool SetChangeWeapon(wstring weaponFilePath);

private:
	wstring rootFilePath;
	bool isRootBoneLock;

	typedef pair<wstring, bool> Pair;
	vector<Pair> rootBoneLockVec;

// 검 변경 관련
private:
	wstring swordName;

// 바이너리 입출력 관련
public:


private:
	BinaryWriter* bw;
	BinaryReader* br;
};
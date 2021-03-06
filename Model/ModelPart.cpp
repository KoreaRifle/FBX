#include "../stdafx.h"
#include "ModelPart.h"
#include "Model.h"
#include "ModelMaterial.h"
#include "ModelBoneWeights.h"
#include "ModelBuffer.h"

UINT ModelPart::count = 0;
ModelPart::ModelPart(Model* model, ModelMaterial* material, ModelBuffer* modelBuffer)
	: Shader(L"./Model/Model.fx")
	, model(model), material(material), modelBuffer(modelBuffer)
	, vertexBuffer(NULL), indexBuffer(NULL)
{
	CreateInputLayout(VertexTextureNormalBlend ::desc, VertexTextureNormalBlend::count);

	D3DXMatrixIdentity(&world);
}

ModelPart::~ModelPart()
{
	SAFE_DELETE_ARRAY(vertex);
	SAFE_DELETE_ARRAY(index);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void ModelPart::Update(bool isAnimation)
{
	D3DXMATRIX offset;
	if (isAnimation == true)
	{
		if (isSkinnedModel == true)
			offset = model->GetGeometricOffset();
		else
			offset = model->GetGeometricOffset() * model->GetAnimationTransform();
	}
	else
		offset = model->GetGeometricOffset();

	//world = offset * world;

	worldBuffer->SetWorld(world * offset);

	Shader::Update();
}

void ModelPart::Render()
{
	ID3D11DeviceContext* dc = D3D::GetDeviceContext();

	UINT stride = sizeof(VertexTextureNormalBlend);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	ID3D11ShaderResourceView* diffuseView = material->GetDiffuseView();
	dc->PSSetShaderResources(0, 1, &diffuseView);
	
	Shader::Render();

	dc->DrawIndexed(indexCount, 0, 0);
}

void ModelPart::AddVertex(D3DXVECTOR3 & position, D3DXVECTOR3 & normal, D3DXVECTOR2 & uv, const ModelBoneWeights& boneWeights)
{
	positions.push_back(position);
	normals.push_back(normal);
	uvs.push_back(uv);
	indices.push_back((UINT)indices.size());

	this->boneWeights.push_back(boneWeights);
	if (boneWeights.GetBoneWeightCount() > 0)
		isSkinnedModel = true;
}

//? CreateBuffer 원본
//void ModelPart::CreateBuffer()
//{
//	vertexCount = positions.size();
//	indexCount = indices.size();
//
//	vertex = new VertexTextureNormalBlend[vertexCount];
//	for (UINT i = 0; i < vertexCount; i++)
//	{
//		vertex[i].position = positions[i];
//		vertex[i].normal = normals[i];
//		vertex[i].uv = uvs[i];
//	}
//
//	for (UINT i = 0; i < boneWeights.size(); i++)
//	{
//		ModelBlendWeights weight = boneWeights[i].GetBlendWeights();
//		vertex[i].blendIndices = weight.BlendIndices;
//		vertex[i].blendWeights = weight.BlendWeights;
//	}
//
//	index = new UINT[indexCount];
//	for (UINT i = 0; i < indexCount; i++)
//		index[i] = indices[i];
//
//
//	HRESULT hr;
//	D3D11_BUFFER_DESC desc;
//	D3D11_SUBRESOURCE_DATA data;
//
//
//	//1. Vertex Buffer
//	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.ByteWidth = sizeof(VertexTextureNormalBlend) * vertexCount;
//	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//
//	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
//	data.pSysMem = vertex;
//
//	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
//	assert(SUCCEEDED(hr));
//
//
//	//2. Index Buffer
//	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.ByteWidth = sizeof(UINT) * indexCount;
//	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//
//	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
//	data.pSysMem = index;
//
//	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
//	assert(SUCCEEDED(hr));
//}

//? CreateBuffer 복제본
void ModelPart::CreateBuffer()
{
	vertexCount = positions.size();
	indexCount = indices.size();

	vertex = new VertexTextureNormalBlend[vertexCount];
	for (UINT i = 0; i < vertexCount; i++)
	{
		vertex[i].position = positions[i];
		vertex[i].normal = normals[i];
		vertex[i].uv = uvs[i];
	}

	for (UINT i = 0; i < boneWeights.size(); i++)
	{
		ModelBlendWeights weight = boneWeights[i].GetBlendWeights();
		vertex[i].blendIndices = weight.BlendIndices;
		vertex[i].blendWeights = weight.BlendWeights;
	}

	index = new UINT[indexCount];
	for (UINT i = 0; i < indexCount; i++)
		index[i] = indices[i];


	HRESULT hr;
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;


	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexTextureNormalBlend) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertex;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	//2. Index Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = index;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));
}

void ModelPart::SetWorldTransform(D3DXMATRIX & world)
{
	this->world = world;
}

void ModelPart::GetCollisionBoxMinMaxValue(D3DXVECTOR3 * collisionBoxMin, D3DXVECTOR3 * collisionBoxMax)
{
	// 원형 콜리전 박스 제작을 위한 파츠별 최대/최소값 확인
	D3DXVECTOR3 tempMinValue, tempMaxValue;
	tempMinValue = tempMaxValue = positions[0];

	for (size_t i = 0; i < positions.size(); i++)
	{
		// 최소값 계산
		if (positions[i].x < tempMinValue.x) tempMinValue.x = positions[i].x;
		if (positions[i].y < tempMinValue.y) tempMinValue.y = positions[i].y;
		if (positions[i].z < tempMinValue.z) tempMinValue.z = positions[i].z;

		if (positions[i].x > tempMaxValue.x) tempMaxValue.x = positions[i].x;
		if (positions[i].y > tempMaxValue.y) tempMaxValue.y = positions[i].y;
		if (positions[i].z > tempMaxValue.z) tempMaxValue.z = positions[i].z;
	}
 	*collisionBoxMin = tempMinValue;
	*collisionBoxMax = tempMaxValue;
}

void ModelPart::SetBinaryFile(BinaryWriter * bw)
{
	this->bw = bw;
	WriteBinaryFile();
}

void ModelPart::SetBInaryFile(BinaryReader * br)
{
	this->br = br;
	ReadBinaryFile();
}

void ModelPart::WriteBinaryFile()
{
	/*bw->UInt((UINT)positions.size());
	bw->UInt((UINT)normals.size());
	bw->UInt((UINT)uvs.size());*/

	for (size_t i = 0; i < positions.size(); i++)
	{
		bw->Vector3(positions[i]);
		bw->Vector3(normals[i]);
		bw->Vector2(uvs[i]);
	}
		
	bw->Matrix(world);
	
	//? 본 정보들도 들어감
	for (UINT i = 0; i < boneWeights.size(); i++)
	{
		ModelBlendWeights weight = boneWeights[i].GetBlendWeights();
		bw->Vector4(weight.BlendIndices);
		bw->Vector4(weight.BlendWeights);
	}
	string test = to_string(count);
	test += ".txt";
	ofstream fout;
	fout.open("./Binary/" + test);
	fout << positions.size() << endl;
	fout << normals.size() << endl;
	fout << uvs.size() << endl;
	for (size_t i = 0; i < positions.size(); i++)
	{
		fout << positions[i] << endl;
		fout << normals[i] << endl;
		fout << uvs[i] << endl;
	}
	fout << endl;
	count++;
}

void ModelPart::ReadBinaryFile()
{
	size_t positionsSize = (size_t)br->UInt();
	size_t normalsSize = (size_t)br->UInt();
	size_t uvsSize = (size_t)br->UInt();

	positions.push_back(br->Vector3());
}

void ModelPart::GetBinaryVector(D3DXVECTOR3 * vecPos, D3DXVECTOR3 * vecNormal, D3DXVECTOR2 * vecUv)
{
	*vecPos = br->Vector3();
	*vecNormal = br->Vector3();
	*vecUv = br->Vector2();
}

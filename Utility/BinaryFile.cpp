#include "../stdafx.h"
#include "BinaryFile.h"

//////////////////////////////////////////////////////////////////////////

BinaryWriter::BinaryWriter()
	: fileHandle(NULL), size(0)
{

}

BinaryWriter::~BinaryWriter()
{
	
}

void BinaryWriter::Open(wstring filePath, UINT openOption)
{
	assert(filePath.length() > 0);
	fileHandle = CreateFile
	(
		filePath.c_str()
		, GENERIC_WRITE
		, 0
		, NULL
		, openOption
		, FILE_ATTRIBUTE_NORMAL
		, NULL
	);


	bool isChecked = fileHandle != INVALID_HANDLE_VALUE;
	assert(isChecked);
}

void BinaryWriter::Close()
{
	if (fileHandle != NULL)
	{
		CloseHandle(fileHandle);
		fileHandle = NULL;
	}
}

void BinaryWriter::Bool(bool data)
{
	WriteFile(fileHandle, &data, sizeof(bool), &size, NULL);
}

void BinaryWriter::Word(WORD data)
{
	WriteFile(fileHandle, &data, sizeof(WORD), &size, NULL);
}

void BinaryWriter::Int(int data)
{
	WriteFile(fileHandle, &data, sizeof(int), &size, NULL);
}

void BinaryWriter::UInt(UINT data)
{
	WriteFile(fileHandle, &data, sizeof(UINT), &size, NULL);
}

void BinaryWriter::Float(float data)
{
	WriteFile(fileHandle, &data, sizeof(float), &size, NULL);
}

void BinaryWriter::Double(double data)
{
	WriteFile(fileHandle, &data, sizeof(double), &size, NULL);
}

void BinaryWriter::Vector2(D3DXVECTOR2& data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXVECTOR2), &size, NULL);
}

void BinaryWriter::Vector3(D3DXVECTOR3& data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXVECTOR3), &size, NULL);
}

void BinaryWriter::Vector4(D3DXVECTOR4& data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXVECTOR4), &size, NULL);
}

void BinaryWriter::Color3f(D3DXCOLOR& data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXCOLOR) - 4, &size, NULL);
}

void BinaryWriter::Color4f(D3DXCOLOR& data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXCOLOR), &size, NULL);
}

void BinaryWriter::Matrix(D3DXMATRIX& data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXMATRIX), &size, NULL);
}

//////////////////////////////////////////////////////////////////////////

BinaryReader::BinaryReader()
	: fileHandle(NULL), size(0)
{

}

BinaryReader::~BinaryReader()
{
	
}

void BinaryReader::Open(wstring filePath)
{
	assert(filePath.length() > 0);
	fileHandle = CreateFile
	(
		filePath.c_str()
		, GENERIC_READ
		, FILE_SHARE_READ
		, NULL
		, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL
		, NULL
	);


	bool isChecked = fileHandle != INVALID_HANDLE_VALUE;
	assert(isChecked);
}

void BinaryReader::Close()
{
	if (fileHandle != NULL)
	{
		CloseHandle(fileHandle);
		fileHandle = NULL;
	}
}

bool BinaryReader::Bool()
{
	bool temp = false;
	ReadFile(fileHandle, &temp, sizeof(bool), &size, NULL);

	return temp;
}

WORD BinaryReader::Word()
{
	WORD temp = 0;
	ReadFile(fileHandle, &temp, sizeof(WORD), &size, NULL);

	return temp;
}

int BinaryReader::Int()
{
	int temp = 0;
	ReadFile(fileHandle, &temp, sizeof(int), &size, NULL);

	return temp;
}

UINT BinaryReader::UInt()
{
	UINT temp = 0;
	ReadFile(fileHandle, &temp, sizeof(UINT), &size, NULL);

	return temp;
}

float BinaryReader::Float()
{
	float temp = 0.0f;
	ReadFile(fileHandle, &temp, sizeof(float), &size, NULL);

	return temp;
}

double BinaryReader::Double()
{
	double temp = 0.0f;
	ReadFile(fileHandle, &temp, sizeof(double), &size, NULL);

	return temp;
}

D3DXVECTOR2 BinaryReader::Vector2()
{
	return D3DXVECTOR2(Float(), Float());
}

D3DXVECTOR3 BinaryReader::Vector3()
{
	return D3DXVECTOR3(Float(), Float(), Float());
}

D3DXVECTOR4 BinaryReader::Vector4()
{
	return D3DXVECTOR4(Float(), Float(), Float(), Float());
}

D3DXCOLOR BinaryReader::Color3f()
{
	return D3DXCOLOR(Float(), Float(), Float(), 1.0f);
}

D3DXCOLOR BinaryReader::Color4f()
{
	return D3DXCOLOR(Float(), Float(), Float(), Float());
}

D3DXMATRIX BinaryReader::Matrix()
{
	D3DXMATRIX matrix;
	matrix._11 = Float();	matrix._12 = Float();	matrix._13 = Float();	matrix._14 = Float();
	matrix._21 = Float();	matrix._22 = Float();	matrix._23 = Float();	matrix._24 = Float();
	matrix._31 = Float();	matrix._32 = Float();	matrix._33 = Float();	matrix._34 = Float();
	matrix._41 = Float();	matrix._42 = Float();	matrix._43 = Float();	matrix._44 = Float();

	return D3DXMATRIX();
}
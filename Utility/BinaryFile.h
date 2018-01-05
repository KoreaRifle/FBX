#pragma once

class BinaryWriter
{
public:
	BinaryWriter();
	~BinaryWriter();

	void Open(wstring filePath, UINT openOption= CREATE_ALWAYS);
	void Close();

	void Bool(bool data);
	void Word(WORD data);
	void Int(int data);
	void UInt(UINT data);
	void Float(float data);
	void Double(double data);

	void Vector2(D3DXVECTOR2& data);
	void Vector3(D3DXVECTOR3& data);
	void Vector4(D3DXVECTOR4& data);
	void Color3f(D3DXCOLOR& data);
	void Color4f(D3DXCOLOR& data);
	void Matrix(D3DXMATRIX& data);

protected:
	HANDLE fileHandle;
	DWORD size;
};

//////////////////////////////////////////////////////////////////////////

class BinaryReader
{
public:
	BinaryReader();
	~BinaryReader();

	void Open(wstring filePath);
	void Close();

	bool Bool();
	WORD Word();
	int Int();
	UINT UInt();
	float Float();
	double Double();

	D3DXVECTOR2 Vector2();
	D3DXVECTOR3 Vector3();
	D3DXVECTOR4 Vector4();
	D3DXCOLOR Color3f();
	D3DXCOLOR Color4f();
	D3DXMATRIX Matrix();

protected:
	HANDLE fileHandle;
	DWORD size;
};
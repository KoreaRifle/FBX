#pragma once

class Blender
{
public:
	static Blender* Get();
	static void Delete();

	void SetLinear();
	void SetOff();

private:
	static Blender* instance;

	Blender();
	~Blender();

	ID3D11BlendState* linearState;
	ID3D11BlendState* offState;
};
#pragma once

class UserInterface
{
public:
	static UserInterface* Get();
	static void Delete();

	static void AddCamera(D3DXVECTOR3* position);

	static void AddSun(D3DXVECTOR3* lightDir);
	static void AddMaterial(string groupName, D3DXCOLOR* ambient, D3DXCOLOR* diffuse);
	
	static void AddSkydome(D3DXCOLOR* apex, D3DXCOLOR* center);

	static void AddFbxModelScale(float* scale);

private:
	static UserInterface* instance;
	static TwBar* bar;

	UserInterface();
	~UserInterface();

	void FillMode(void* value);
	UINT fillMode;
};
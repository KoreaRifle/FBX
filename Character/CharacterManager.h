#pragma once

class Player;
class Enemy;
class CharacterManager
{
public:
	CharacterManager();
	~CharacterManager();

	void CreateObject();

	virtual void Update();
	virtual void Render();

protected:
	void CheckMonsterCreate();

	/*template <typename T>
	void SetLocation(T, D3DXVECTOR3 location);*/
	void SetLocation(wstring className, D3DXVECTOR3 location);
	D3DXVECTOR3 GetLocation(wstring className);

private:
	Player* player;
	Enemy* monster;

private:
	static int monsterCount;

	vector<D3DXVECTOR3> location;
	D3DXVECTOR3 playerLocation;
	D3DXVECTOR3 enemyLocation;
};

#pragma once
#include "../../Library/GameObject.h"

class TrashEnemyManager;
class Wave :public GameObject
{
public:
	Wave();
	~Wave();
	void Update();
	void Draw();
	/// <summary>
	/// 初めて生成されるときに使用
	/// </summary>
	void FirstRespown();

	bool GetBossWave();

private:
	void EnemySpawn();
	void CooperateAttack();
	TrashEnemyManager* tEnemyManager;
	//雑魚敵の最大スポーン数
	const int SPAWN_MAX = 20;
	//最初のスポーン数
	const float INITIALSPAWN = 8;
	//最大ウェーブ数
	const int WAVE_MAX = 3;
	const VECTOR3 SPWNPOS = VECTOR3(0, 0, 2500);
	int waveNow;

	float battleCounter;
	int spawn;

	bool isCooperate;

	bool bossCreate;
	bool first;
	bool bossWave;

	struct SpawnData
	{
		int MeleeEnemyCounter;
		int RangedEnemyCounter;
	};
	std::vector<SpawnData> spawnData
	{
		{5,0},
		{13,5},
	};
};
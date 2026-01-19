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

	void FirstRespown();

private:
	void EnemySpawn();
	void CooperateAttack();
	TrashEnemyManager* tEnemyManager;
	//雑魚敵の最大スポーン数
	const int SPAWN_MAX = 20;
	//最初のスポーン数
	const float INITIALSPAWN = 8;
	//最大ウェーブ数
	const int WAVE_MAX = 2;
	const VECTOR3 SPWNPOS = VECTOR3(0, 0, 2500);
	int waveNow;

	float battleCounter;
	int spawn;

	bool isCooperate;

	bool bossCreate;
	bool first;
};
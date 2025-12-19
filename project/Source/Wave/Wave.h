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
private:
	void EnemySpawn();
	TrashEnemyManager* tEnemyManager;

	const int SPAWN_MAX = 25;
	const int INITIALSPAWN = 5;
	const int WAVE_MAX = 2;

	int waveNow;
	float battleCounter;
	int spawn;
};
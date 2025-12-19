#include "Wave.h"
#include "../Enemy/TrashEnemy/TrashEnemyManager.h"

Wave::Wave()
{
	tEnemyManager = new TrashEnemyManager;
	tEnemyManager->CreateEnemy(VZero, INITIALSPAWN);
	waveNow = 1;
	battleCounter = 0;
	spawn = SPAWN_MAX;
}

Wave::~Wave()
{
}

void Wave::Update()
{
	EnemySpawn();
	
}

void Wave::Draw()
{
}

void Wave::EnemySpawn()
{
	if (spawn <= 5|| waveNow >= WAVE_MAX)
		return;

	//一個前のウェーブで敵を倒した速度によって敵の出す数を変えるためのカウンター
	battleCounter += Time::DeltaTimeRate();

	if (battleCounter >= INITIALSPAWN)
	{
		spawn--;
		battleCounter = 0;
	}

	if (tEnemyManager->EnemyList().size() <= 0)
	{
		tEnemyManager->CreateEnemy(VZero, spawn);
		battleCounter = 0;
		spawn = SPAWN_MAX;
		waveNow++;
	}
}

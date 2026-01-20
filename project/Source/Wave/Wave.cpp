#include "Wave.h"
#include "../Enemy/TrashEnemy/TrashEnemyManager.h"
#include "../Enemy/EnemyManager.h"

Wave::Wave()
{
	tEnemyManager = new TrashEnemyManager;
	
	waveNow = 1;
	battleCounter = 0;
	spawn = SPAWN_MAX;
	isCooperate = false;
	bossCreate = true;
	first = false;
}

Wave::~Wave()
{
}

void Wave::Update()
{
	if (!first) {
		return;
	}
	EnemySpawn();
	CooperateAttack();
	if (waveNow == 3) {
		if (bossCreate) {
			FindGameObject<EnemyManager>()->CreateBoss();
			bossCreate = false;
		}
	}
}

void Wave::Draw()
{
}

void Wave::FirstRespown()
{
	tEnemyManager->CreateEnemy(SPWNPOS, INITIALSPAWN);
	first = true;
}

void Wave::EnemySpawn()
{
	if (waveNow > WAVE_MAX)
		return;

	//一個前のウェーブで敵を倒した速度によって敵の出す数を変えるためのカウンター
	battleCounter += Time::DeltaTimeRate();

	if (spawn >= 5 && battleCounter >= 10)
	{
		spawn--;
		battleCounter = 0;
	}

	if (tEnemyManager->EnemyList().size() <= 0)
	{
		if (waveNow < WAVE_MAX) {
			tEnemyManager->CreateEnemy(SPWNPOS, spawn);
		}

		isCooperate = false;
		battleCounter = 0;
		spawn = SPAWN_MAX;
		waveNow++;
	}
}

void Wave::CooperateAttack()
{
	if (/*waveNow != 2 ||*/ tEnemyManager->EnemyList().size() > 4 || isCooperate)
		return;

	tEnemyManager->Cooperate(StateID::COOPERATEATTACK1);
	isCooperate = true;
}

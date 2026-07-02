#include "Wave.h"
#include "../Enemy/TrashEnemy/TrashEnemyManager.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/Boss/BossCreater.h"
#include "../GameControler/GameControler.h"
#include "../Common/Fead/Fead.h"
#include "../Stage/StageSelectData.h"
#include "../Common/JsonReader/JsonReader.h"

Wave::Wave()
{
	tEnemyManager = new TrashEnemyManager;
	const StageData stageData = StageSelectData::GetInstance()->GetNowStageData();
	stageNum = stageData.id;

	JsonReader json; 
	json.Load("data/json/WaveData.json");
	//json.Data();
	JSON jsonData = json.Data();

	stageMax = jsonData["WaveEnemyData"][std::to_string(stageNum)].size();
	SpawnData data;

	for (int i = 0; i < stageMax; i++)
	{
		std::string a = "Wave" + std::to_string((i + 1));
		data.SetData(jsonData["WaveEnemyData"][std::to_string(stageNum)][ a]["melee"], jsonData["WaveEnemyData"][std::to_string(stageNum)][a]["range"]);
		spawnData.push_back(data);
	}

	waveNow = 0;
	battleCounter = 0;
	isCooperate = false;
	bossCreate = true;
	first = false;
	bossWave = false;
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
	
	/*CooperateAttack();
	RangedCooperateAttack();*/
}

void Wave::Draw()
{
}

void Wave::FirstRespown()
{
	first = true;
}

bool Wave::GetBossWave()
{
	return bossWave;
}

void Wave::EnemySpawn()
{
	if (waveNow > stageMax)
		return;

	int _counter = tEnemyManager->GetActiveEnemy();
	if (_counter <= 0)
	{
		waveNow++;

		if(waveNow <= stageMax)
			tEnemyManager->CreateEnemy(SPWNPOS, spawnData[waveNow - 1].MeleeEnemyCounter, spawnData[waveNow - 1].RangedEnemyCounter);
		
		else/*if (waveNow == stageMax)*/
			if (bossCreate)
			{
				FindGameObject<BossCreater>()->CreateBoss();
				FindGameObject<GameControler>()->ChangeState(GameControler::GameState::BOSS_PLAY_BEFORE);

				FindGameObject<Fead>()->FeadIn(1.0f, 0x000000, Easing::EaseIn<int>);
				bossCreate = false;
				bossWave = true;
			}
		isCooperate = false;
	}
}

//void Wave::CooperateAttack()
//{
//	int counter = tEnemyManager->GetMeleeActiveEnemy();
//
//	if (counter > 4 || isCooperate)
//		return;
//
//	if (waveNow == 1)
//	{
//		tEnemyManager->Cooperate();
//		isCooperate = true;
//	}
//}
//
//void Wave::RangedCooperateAttack()
//{
//	if (waveNow != 2)
//		return;
//	int counter = tEnemyManager->GetMeleeActiveEnemy();
//
//	if (counter <= 5)
//		isCooperate = false;
//
//	if (counter > 10 || isCooperate)
//		return;
//
//	tEnemyManager->SetStartRangedAttack(true);
//	isCooperate = true;
//}

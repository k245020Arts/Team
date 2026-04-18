#include "T_EnemyWaitSee.h"
#include "../TrashEnemy.h"

T_EnemyWaitSee::T_EnemyWaitSee()
{
	animId = ID::TE_IDOL;

	attackCounter = 0.0f;
	targetPos = VZero;
	isLeader = false;
}

T_EnemyWaitSee::~T_EnemyWaitSee()
{
}

void T_EnemyWaitSee::Update()
{

}

void T_EnemyWaitSee::Start()
{
	const TrashEnemy* enemy = GetBase<TrashEnemy>();
	
	switch (enemy->enemyType)
	{
	case enemy->EnemyType::RANGED:
		
		break;
	case enemy->EnemyType::RANGED_LEADER:
		isLeader = true;
		break;
	default:
		break;
	}
}

void T_EnemyWaitSee::Finish()
{
}

#include "T_EnemyStaySky.h"
#include "../TrashEnemy.h"
#include "../../../State/StateManager.h"

T_EnemyStaySky::T_EnemyStaySky()
{
}

T_EnemyStaySky::~T_EnemyStaySky()
{
}

void T_EnemyStaySky::Update()
{

}

void T_EnemyStaySky::Start()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	if (enemy->GetEnemyType() == enemy->EnemyType::RANGED_LEADER)
		isLeader = true;

	EnemyStateBase::Start();

}

void T_EnemyStaySky::Finish()
{
}

//void T_EnemyStaySky::LeaderMove(TrashEnemy* _enemy)
//{
//	
//}
//
//void T_EnemyStaySky::RangedMove(TrashEnemy* _enemy)
//{
//
//}

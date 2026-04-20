#include "T_EnemyWaitSee.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"

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
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	enemy->LookTarget(enemy->enemyBaseComponent.playerObj->GetTransform()->position);

	if (!isLeader)
		NormalMove(enemy);
	else
		ReaderMove(enemy);
}

void T_EnemyWaitSee::Start()
{
	EnemyStateBase::Start();
	const TrashEnemy* enemy = GetBase<TrashEnemy>();
	
	switch (enemy->enemyType)
	{
	case enemy->EnemyType::RANGED:
		
		break;
	case enemy->EnemyType::RANGED_LEADER:
		targetPos = enemy->TargetPoint();
		isLeader = true;
		break;
	default:
		break;
	}
}

void T_EnemyWaitSee::Finish()
{
}

void T_EnemyWaitSee::ReaderMove(TrashEnemy* _enemy)
{
	float speed = 10.0f;
	targetPos = _enemy->TargetPoint();
	const VECTOR3 enePos = _enemy->GetPos();

	if (VSize(targetPos - enePos) < 400.0f)
		return;

	VECTOR3 dir = VNorm(targetPos - enePos);
	_enemy->GetEnemyObj()->GetTransform()->position+= dir * speed;

}

void T_EnemyWaitSee::NormalMove(TrashEnemy* _enemy)
{
}

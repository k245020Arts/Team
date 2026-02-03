#include "T_EnemyRUN.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"

T_EnemyRun::T_EnemyRun()
{
	animId = ID::TE_RUN;
	string = Function::GetClassNameC<T_EnemyRun>();

	targetPos = VZero;
}

T_EnemyRun::~T_EnemyRun()
{
}

void T_EnemyRun::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();

	if (e->IsPlayerSpecialMove())
		return; 

	if (!e->isCooperateAtk || e->isMovingToPlayer)
		targetPos = e->enemyBaseComponent.playerObj->GetTransform()->position;

	rotation = e->obj->GetTransform()->rotation;
	e->LookTarget(targetPos);

	const float ROTY = -rotation.y - 0.5f * DX_PI_F;

	e->GetEnemyObj()->GetTransform()->position.x += e->eStatus->GetStatus().runSpeed * cosf(ROTY);
	e->GetEnemyObj()->GetTransform()->position.z += e->eStatus->GetStatus().runSpeed * sinf(ROTY);

	VECTOR3 targetVec = targetPos - e->obj->GetTransform()->position;
	
	if (targetVec.Size() <= e->eStatus->GetStatus().atkRang)
	{
		if (!e->IsMovingToPlayer())
			e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_STANDBY);
		else 
			e->enemyBaseComponent.state->ChangeState(StateID::COOPERATEATTACK1);
	}
}

void T_EnemyRun::Start()
{
	EnemyStateBase::Start();
	TrashEnemy* e = GetBase<TrashEnemy>();
	if (!e->isCooperateAtk|| e->isMovingToPlayer)
		targetPos = e->enemyBaseComponent.playerObj->GetTransform()->position;
	else
		targetPos = e->wayPoint;
}

void T_EnemyRun::Finish()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	if (!e->isCooperateAtk)
		e->isAttack = true;
}

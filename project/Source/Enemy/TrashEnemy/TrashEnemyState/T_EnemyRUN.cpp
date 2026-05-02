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

	motionSpeed = 0;
}

T_EnemyRun::~T_EnemyRun()
{
}

void T_EnemyRun::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	if (enemy->IsPlayerSpecialMove())
	{
		enemy->enemyBaseComponent.anim->SetPlaySpeed(0);
		return;
	}
	else
		enemy->enemyBaseComponent.anim->SetPlaySpeed(motionSpeed);

	if (!enemy->isCooperateAtk )
		targetPos = enemy->targetPoint;
	else if(enemy->isMovingToPlayer)
		enemy->enemyBaseComponent.playerObj->GetTransform()->position;

	rotation = enemy->obj->GetTransform()->rotation;
	enemy->LookTarget(targetPos);

	const float ROTY = -rotation.y - 0.5f * DX_PI_F;

	enemy->GetEnemyObj()->GetTransform()->position.x += enemy->eStatus->GetStatus().runSpeed * cosf(ROTY);
	enemy->GetEnemyObj()->GetTransform()->position.z += enemy->eStatus->GetStatus().runSpeed * sinf(ROTY);

	VECTOR3 targetVec = targetPos - enemy->obj->GetTransform()->position;
	
	if (targetVec.Size() <= enemy->eStatus->GetStatus().atkRang)
	{
		if (!enemy->IsMovingToPlayer())
			enemy->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_STANDBY);
		else 
			enemy->enemyBaseComponent.state->ChangeState(enemy->GetNextCooperateID());
	}
}

void T_EnemyRun::Start()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	if (enemy->enemyType == enemy->EnemyType::MELEE)
	{
		if (!enemy->isCooperateAtk)
			targetPos = enemy->targetPoint;
		else if (enemy->isMovingToPlayer)
			targetPos = enemy->enemyBaseComponent.playerObj->GetTransform()->position;
		else
			targetPos = enemy->cooperateWayPoint;
	}
	else
	{
		enemy->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_WAITSEE);
	}

	motionSpeed = enemy->enemyBaseComponent.anim->GetPlaySpeed();

	enemy->isRunState = true;

	EnemyStateBase::Start();
}

void T_EnemyRun::Finish()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	
	enemy->isRunState = false;
}

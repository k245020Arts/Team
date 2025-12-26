#include "T_EnemyIdol.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"

T_EnemyIdol::T_EnemyIdol()
{
	animId = ID::TE_IDOL;
	string = Function::GetClassNameC<T_EnemyIdol>();
	detectionRange = 0;
}

T_EnemyIdol::~T_EnemyIdol()
{
}

void T_EnemyIdol::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();

	VECTOR3 targetVec = e->obj->GetTransform()->position - e->enemyBaseComponent.playerObj->GetTransform()->position;
		//EnemyTransform->position - e->enemyBaseComponent.playerObj->GetTransform()->position;
	detectionRange += Time::DeltaTimeRate() * 50;
	if (targetVec.Size() < e->eStatus->GetStatus().chaseRange + detectionRange)
		e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_RUN_S);
}

void T_EnemyIdol::Draw()
{
}

void T_EnemyIdol::Start()
{
	EnemyStateBase::Start();
}

void T_EnemyIdol::Finish()
{
}

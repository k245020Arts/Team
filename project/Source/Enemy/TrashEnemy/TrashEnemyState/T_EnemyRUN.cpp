#include "T_EnemyRUN.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"

T_EnemyRun::T_EnemyRun()
{
	animId = ID::TE_RUN;
	string = Function::GetClassNameC<T_EnemyRun>();
}

T_EnemyRun::~T_EnemyRun()
{
}

void T_EnemyRun::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	rotation = e->obj->GetTransform()->rotation;
	e->LookTarget();
	const float ROTY = -rotation.y - 0.5f * DX_PI_F;

	//スピードをポジションに足す
	e->Move(e->speed, 2000);

	VECTOR3 targetVec = e->obj->GetTransform()->position - e->targetPos;//e->enemyBaseComponent.playerObj->GetTransform()->position;
	//static const float  RANG = 1200.0f;

	if (targetVec.Size() <= e->eStatus->GetStatus().atkRang)
		e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_STANDBY);
}

void T_EnemyRun::Start()
{
	EnemyStateBase::Start();
	TrashEnemy* e = GetBase<TrashEnemy>();
}

void T_EnemyRun::Finish()
{
}

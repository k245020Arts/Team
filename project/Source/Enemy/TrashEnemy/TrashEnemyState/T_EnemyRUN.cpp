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

	//移動の計算
	velocity.x = speed * cosf(ROTY);
	velocity.z = speed * sinf(ROTY);

	//計算したものをポジションに足す
	e->obj->GetTransform()->position += velocity;

	VECTOR3 targetVec = e->obj->GetTransform()->position - e->enemyBaseComponent.playerObj->GetTransform()->position;
	static const float  RANG = 1200.0f;

	if (targetVec.Size() <= e->eStatus->GetStatus().atkRang)
		e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_ATTACK_S);
}

void T_EnemyRun::Start()
{
	EnemyStateBase::Start();
	TrashEnemy* e = GetBase<TrashEnemy>();
	speed = e->eStatus->GetStatus().runSpeed;
}

void T_EnemyRun::Finish()
{
}

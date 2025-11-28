#include "T_EnemyRUN.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"

T_EnemyRun::T_EnemyRun()
{
	animId = ID::E_RUN;
	string = Function::GetClassNameC<T_EnemyRun>();
}

T_EnemyRun::~T_EnemyRun()
{
}

void T_EnemyRun::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	rotation = e->obj->GetTransform()->rotation;
	e->LookPlayer();
	const float ROTY = -rotation.y - 0.5f * DX_PI_F;

	//移動の計算
	velocity.x = 10 * cosf(ROTY);
	velocity.z = 10 * sinf(ROTY);

	//計算したものをポジションに足す
	e->obj->GetTransform()->position += velocity;

	VECTOR3 targetVec = e->obj->GetTransform()->position - e->enemyBaseComponent.playerObj->GetTransform()->position;
	if (targetVec.Size() <= 500)
		e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_ATTACK_S);
}

void T_EnemyRun::Start()
{
	EnemyStateBase::Start();
}

void T_EnemyRun::Finish()
{
}

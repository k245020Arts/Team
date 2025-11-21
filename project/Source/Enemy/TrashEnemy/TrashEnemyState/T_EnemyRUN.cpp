#include "T_EnemyRUN.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"

T_EnemyRun::T_EnemyRun()
{
}

T_EnemyRun::~T_EnemyRun()
{
}

void T_EnemyRun::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	rotation = e->obj->GetTransform()->rotation;
	const float ROTY = -rotation.y - 0.5f * DX_PI_F;

	//移動の計算
	velocity.x = 5 * cosf(ROTY);
	velocity.z = 5 * sinf(ROTY);

	//計算したものをボスのポジションに足す
	e->obj->GetTransform()->position += velocity;

	VECTOR3 targetVec = e->obj->GetTransform()->position - e->enemyBaseComponent.playerObj->GetTransform()->position;
	/*if (targetVec.Size() <= 300)
		e->enemyBaseComponent.state->ChangeState(ID::E_ATTACK1);*/
}

void T_EnemyRun::Start()
{
}

void T_EnemyRun::Finish()
{
}

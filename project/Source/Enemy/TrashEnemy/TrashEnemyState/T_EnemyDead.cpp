#include "T_EnemyDead.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"

T_EnemyDead::T_EnemyDead()
{
	animId = ID::E_DIE;
	string = Function::GetClassNameC<T_EnemyDead>();
}

T_EnemyDead::~T_EnemyDead()
{
}

void T_EnemyDead::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();

	float rotY = e->GetEnemyObj()->GetTransform()->rotation.y;
	e->GetEnemyObj()->GetTransform()->position.x -= 10 * cosf(rotY - 0.5f * DX_PI_F);
	//e->GetEnemyObj()->GetTransform()->position.y += 5.0f;
	e->GetEnemyObj()->GetTransform()->position.z -= 10 * sinf(rotY - 0.5f * DX_PI_F);

	if (e->enemyBaseComponent.anim->IsFinish())
		e->active = false;
}

void T_EnemyDead::Draw()
{
}

void T_EnemyDead::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	EnemyStateBase::Start();
	e->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
}

void T_EnemyDead::Finish()
{
}

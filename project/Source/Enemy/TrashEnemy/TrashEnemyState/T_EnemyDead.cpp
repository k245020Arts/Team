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
	
	if (e->enemyBaseComponent.anim->IsFinish())
		e->active = false;
}

void T_EnemyDead::Draw()
{
}

void T_EnemyDead::Start()
{
	EnemyStateBase::Start();
}

void T_EnemyDead::Finish()
{
}

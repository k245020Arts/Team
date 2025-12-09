#include "T_EnemyDead.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"

T_EnemyDead::T_EnemyDead()
{
	//animId = ID::TE_IDOL;
	string = Function::GetClassNameC<T_EnemyDead>();
}

T_EnemyDead::~T_EnemyDead()
{
}

void T_EnemyDead::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	counter ++;

	if(counter>=120)
		e->active = false;
}

void T_EnemyDead::Draw()
{
}

void T_EnemyDead::Start()
{
	EnemyStateBase::Start();
	counter = 0;
}

void T_EnemyDead::Finish()
{
}

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

}

void T_EnemyDead::Draw()
{
}

void T_EnemyDead::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	EnemyStateBase::Start();
	e->active = false;
}

void T_EnemyDead::Finish()
{
}

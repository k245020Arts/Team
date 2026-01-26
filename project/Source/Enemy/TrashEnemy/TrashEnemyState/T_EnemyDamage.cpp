#include "T_EnemyDamage.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"

T_EnemyDamage::T_EnemyDamage()
{
	animId = ID::TE_IDOL;
	string = Function::GetClassNameC<T_EnemyDamage>();
}

T_EnemyDamage::~T_EnemyDamage()
{
}

void T_EnemyDamage::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();

	if (e->IsPlayerSpecialMove())
		return;

	if (e->enemyBaseComponent.anim->IsFinish())
		e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_STANDBY);
}

void T_EnemyDamage::Draw()
{

}

void T_EnemyDamage::Start()
{

}

void T_EnemyDamage::Finish()
{

}

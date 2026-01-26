#include "T_EnemyDamage.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"

T_EnemyDamage::T_EnemyDamage()
{
	animId = ID::E_DIE;
	string = Function::GetClassNameC<T_EnemyDamage>();

	motionSpeed = 0;
}

T_EnemyDamage::~T_EnemyDamage()
{
}

void T_EnemyDamage::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();

	if (e->IsPlayerSpecialMove())
		e->enemyBaseComponent.anim->SetPlaySpeed(0);
	else
		e->enemyBaseComponent.anim->SetPlaySpeed(motionSpeed);
		
	if (e->enemyBaseComponent.anim->IsFinish())
		e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_STANDBY);
}

void T_EnemyDamage::Draw()
{

}

void T_EnemyDamage::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	EnemyStateBase::Start();
	motionSpeed = e->enemyBaseComponent.anim->GetPlaySpeed();
}

void T_EnemyDamage::Finish()
{

}

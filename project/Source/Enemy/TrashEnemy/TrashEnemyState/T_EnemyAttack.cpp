#include "T_EnemyAttack.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"

T_EnemyAttack::T_EnemyAttack()
{
	//id = ID::TE_ATTACK;
	animId = ID::B_N_ATTACK3;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));

	damage.hitDamage = 20.0f;
}

T_EnemyAttack::~T_EnemyAttack()
{
}

void T_EnemyAttack::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	const float MSPEED = 60.0f;//ƒ‚[ƒVƒ‡ƒ“‚Ì‘¬“x’²®

	if (e->enemyBaseComponent.anim->IsFinish())
		e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_RUN_S);

	AttackCollsion();
	AttackSound();
	AttackFlash(ID::E_MODEL, 11, "E_AttackV");
	Trail();
}

void T_EnemyAttack::Draw()
{
}

void T_EnemyAttack::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	EnemyStateBase::Start();
	firstColl = true;
}

void T_EnemyAttack::Finish()
{
}

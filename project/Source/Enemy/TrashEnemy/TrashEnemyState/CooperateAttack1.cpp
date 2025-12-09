#include "CooperateAttack1.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"

CooperateAttack1::CooperateAttack1()
{
	animId = ID::TE_ATTACK2;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	damage.damagePattern = BossAttackBase::NO_BACK;
}

CooperateAttack1::~CooperateAttack1()
{
}

void CooperateAttack1::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	EnemyStateBase::Start();
	firstColl = true;
}

void CooperateAttack1::Finish()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	e->attackId = StateID::T_ENEMY_ATTACK_S;
	e->speed=e->eStatus->GetStatus().runSpeed;
}

#include "CooperateAttack1.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"

CooperateAttack1::CooperateAttack1()
{
	string = Function::GetClassNameC<CooperateAttack1>();
	animId = ID::TE_ATTACK2;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	damage.damagePattern = BossAttackBase::BACK;

	damage.hitDamage = 30;
}

CooperateAttack1::~CooperateAttack1()
{
}

void CooperateAttack1::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	EnemyStateBase::Start();
	e->enemyBaseComponent.anim->SetFrame(5.0f);
	firstColl = true;
}

void CooperateAttack1::Finish()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	e->NextId = StateID::T_ENEMY_STANDBY;
	e->speed=e->eStatus->GetStatus().runSpeed;
	e->targetPos = e->enemyBaseComponent.playerObj->GetTransform()->position;
	e->isCooperateAtk = false;
	e->DeleteCollision();
}

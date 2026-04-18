#include "CooperateAttack1.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"

CooperateAttack1::CooperateAttack1()
{
	string = Function::GetClassNameC<CooperateAttack1>();
	animId = ID::TE_ATTACK2;
	attackParam.animID = ID::TE_ATTACK2;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	attackParam.damagePattern = BossAttackBase::BACK;

	attackParam.hitDamage = 30;
	time = 0;
	attackParam.useFlash = true;
	attackParam.attackFlashStartTime = 0.7f;
	attackParam.slowAmout = 0.1f;
	attackParam.slowTime = 0.3f;
	attackParam.speedUpMotionSpeed = 0.3f;
}

CooperateAttack1::~CooperateAttack1()
{
}

void CooperateAttack1::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();

	AttackInformation(e);
	time += Time::DeltaTimeRate();

	if (time >= 1)
		e->isCooperateAtk = false;
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
	e->speed=e->eStatus->GetStatus().runSpeed;
	e->CooperateAtkFinish();
	e->DeleteCollision(&e->attackColl);
}

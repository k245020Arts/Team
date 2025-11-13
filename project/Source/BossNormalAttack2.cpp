#include "BossNormalAttack2.h"
#include "Animator.h"
#include "Boss.h"
#include "stateManager.h"
#include "Easing.h"
#include "BossStatus.h"

BossNormalAttack2::BossNormalAttack2()
{
	id = ID::B_N_ATTACK2;
	animId = ID::B_N_ATTACK2;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
}

BossNormalAttack2::~BossNormalAttack2()
{
}

void BossNormalAttack2::Update()
{
	Boss* boss = GetBase<Boss>();
	if (boss->enemyBaseComponent.anim->GetMaxFrame() - fallFrame <= boss->enemyBaseComponent.anim->GetCurrentFrame())
	{
		if (boss->maxAttack != 0)
			boss->enemyBaseComponent.state->ChangeState(ID::B_ATTACKSORTING);
		else
			boss->enemyBaseComponent.state->ChangeState(ID::B_RUN);
	}
	BossAttackCollsion();
	if (boss->enemyBaseComponent.anim->AnimEventCan()) {
		averageSpeed = 90.0f / attackTime;
		averageSpeed *= boss->obj->GetObjectRate();
		boss->bossTransform->rotation.y -= averageSpeed * DegToRad;
	}
	AttackSound();
	AttackFlash(ID::B_MODEL, boss->BOSS_LEFT_HAND_FRAME, "E_AttackV");
	BossTrail(false);
}

void BossNormalAttack2::Draw()
{
}

void BossNormalAttack2::Start()
{
	Boss* boss = GetBase<Boss>();
	EnemyStateBase::Start();
	firstColl = true;
	hitDamage = boss->bs->GetStatus().normalAttack1;
	fallFrame = boss->bs->GetStatus().fallFrame;
}

void BossNormalAttack2::Finish()
{
	Boss* boss = GetBase<Boss>();
	boss->DeleteCollision();
	boss->enemyBaseComponent.anim->AnimEventReset();
}

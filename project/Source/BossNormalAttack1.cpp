#include "BossNormalAttack1.h"
#include "Animator.h"
#include "Boss.h"
#include "stateManager.h"
#include "Easing.h"
#include "BossStatus.h"

BossNormalAttack1::BossNormalAttack1()
{
	id = ID::B_N_ATTACK1;
	animId = ID::B_N_ATTACK1;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
}

BossNormalAttack1::~BossNormalAttack1()
{
}

void BossNormalAttack1::Update()
{
	Boss* boss = GetBase<Boss>();
	//if (boss->enemyBaseComponent.anim->IsFinish()) {
	if (boss->enemyBaseComponent.anim->GetMaxFrame() - a <= boss->enemyBaseComponent.anim->GetCurrentFrame())
	{
		if (boss->maxAttack != 0)
			boss->enemyBaseComponent.state->ChangeState(ID::B_ATTACKSORTING);
		else
			boss->enemyBaseComponent.state->ChangeState(ID::B_RUN);
	}
	BossAttackCollsion();
	if (boss->enemyBaseComponent.anim->AnimEventCan()) {
		/*float n = boss->enemyBaseComponent.anim->EventFinishTime(animId) - boss->enemyBaseComponent.anim->GetCurrentFrame();
		float rate = attackTime / n;
		float ease = Easing::EaseIn(0.0f, 90.0f, n);*/
		averageSpeed = 90.0f / attackTime;
		averageSpeed *= boss->obj->GetObjectRate();
		boss->bossTransform->rotation.y  += averageSpeed * DegToRad;
	}
	AttackSound();
	AttackFlash(ID::B_MODEL,boss->BOSS_RIGHT_HAND_FRAME, "E_AttackV");
	BossTrail(true);
}

void BossNormalAttack1::Draw()
{
}

void BossNormalAttack1::Start()
{
	Boss* boss = GetBase<Boss>();
	EnemyStateBase::Start();
	firstColl = true;
	boss->enemyBaseComponent.anim->AnimEventReset();
	hitDamage = boss->bs->GetStatus().normalAttack1;
	if (boss->maxAttack == 0)
	{
		boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
		a = 0;
	}
	else
		a = boss->bs->GetStatus().maxA;
}

void BossNormalAttack1::Finish()
{
	Boss* boss = GetBase<Boss>();
	boss->DeleteCollision();
	boss->enemyBaseComponent.anim->AnimEventReset();
	if (boss->maxAttack == 0)
		boss->enemyBaseComponent.anim->SetPlaySpeed(1.2f);
}

#include "BossNormalAttack1.h"
#include "../../../Component/Animator/Animator.h"
#include "../../Boss.h"
#include "../../../State/StateManager.h"
#include "../../../Common/Easing.h"
#include "../BossStatus.h"

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
		boss->bossTransform->rotation.y += averageSpeed * DegToRad;
	}
	AttackSound();
	AttackFlash(ID::B_MODEL, boss->BOSS_RIGHT_HAND_FRAME, "E_AttackV");
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

	fallFrame = boss->bs->GetStatus().fallFrame;
}

void BossNormalAttack1::Finish()
{
	Boss* boss = GetBase<Boss>();
	boss->DeleteCollision();
	boss->enemyBaseComponent.anim->AnimEventReset();
}

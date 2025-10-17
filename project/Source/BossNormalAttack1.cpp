#include "BossNormalAttack1.h"
#include "Animator.h"
#include "Boss.h"
#include "stateManager.h"

BossNormalAttack1::BossNormalAttack1()
{
	id = ID::B_N_ATTACK1;
	animId = ID::B_N_ATTACK1;
	collTrans = Transform(VECTOR3(-100, 50, 100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
}

BossNormalAttack1::~BossNormalAttack1()
{
}

void BossNormalAttack1::Update()
{
	Boss* boss = GetBase<Boss>();
	if (boss->enemyBaseComponent.anim->IsFinish()) {
		boss->enemyBaseComponent.state->ChangeState(ID::B_RUN);
	}
	BossAttackCollsion();
}

void BossNormalAttack1::Draw()
{
}

void BossNormalAttack1::Start()
{
	EnemyStateBase::Start();
	firstColl = true;
}

void BossNormalAttack1::Finish()
{
}

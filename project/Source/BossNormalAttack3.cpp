#include "BossNormalAttack3.h"
#include "Animator.h"
#include "Boss.h"
#include "stateManager.h"
#include "Easing.h"

BossNormalAttack3::BossNormalAttack3()
{
	id = ID::B_N_ATTACK3;
	animId = ID::B_N_ATTACK3;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
}

BossNormalAttack3::~BossNormalAttack3()
{
}

void BossNormalAttack3::Update()
{
	Boss* boss = GetBase<Boss>();
	//‚Ç‚±‚Ü‚ÅƒvƒŒƒCƒ„[‚Ì•û‚ðŒ©‚é‚©

	if (boss->enemyBaseComponent.anim->IsFinish()) {
		if (boss->maxAttack != 0)
			boss->enemyBaseComponent.state->ChangeState(ID::B_ATTACKSORTING);
		else
			boss->enemyBaseComponent.state->ChangeState(ID::B_RUN);
	}
	BossAttackCollsion();
	AttackSound();
	AttackFlash(ID::B_MODEL, boss->BOSS_LEFT_HAND_FRAME, "E_AttackV");
	BossTrail(false);
}

void BossNormalAttack3::Draw()
{
}

void BossNormalAttack3::Start()
{
	Boss* boss = GetBase<Boss>();

	EnemyStateBase::Start();
	firstColl = true;
	counter = 0;

	if (boss->maxAttack == 0)
		boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
}

void BossNormalAttack3::Finish()
{
	Boss* boss = GetBase<Boss>();
	if (boss->maxAttack == 0)
		boss->enemyBaseComponent.anim->SetPlaySpeed(1.2f);
}

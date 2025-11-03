#include "BossNormalAttack3.h"
#include "Animator.h"
#include "Boss.h"
#include "stateManager.h"
#include "Easing.h"
#include "BossStatus.h"

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
	Boss* b = GetBase<Boss>();
	//Ç«Ç±Ç‹Ç≈ÉvÉåÉCÉÑÅ[ÇÃï˚Çå©ÇÈÇ©
	counter++;
	if (counter < 50)
		b->LookPlayer();

	//if (b->enemyBaseComponent.anim->IsFinish()) {
	if (b->enemyBaseComponent.anim->GetMaxFrame() - a <= b->enemyBaseComponent.anim->GetCurrentFrame())
	{
		if (b->maxAttack != 0)
			b->enemyBaseComponent.state->ChangeState(ID::B_ATTACKSORTING);
		else
			b->enemyBaseComponent.state->ChangeState(ID::B_RUN);
	}
	BossAttackCollsion();
	AttackSound();
	AttackFlash(ID::B_MODEL, b->BOSS_LEFT_HAND_FRAME, "E_AttackV");
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
	{
		boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
		a = 0;
	}
	else
		a = boss->bs->GetStatus().maxA - 20;
}

void BossNormalAttack3::Finish()
{
	Boss* boss = GetBase<Boss>();
	if (boss->maxAttack == 0)
		boss->enemyBaseComponent.anim->SetPlaySpeed(1.2f);
}

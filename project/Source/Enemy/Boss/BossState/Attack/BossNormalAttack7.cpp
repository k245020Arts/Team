#include "BossNormalAttack7.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Common/Easing.h"
#include "../BossStatus.h"
#include "../../../../Component/Physics/Physics.h"
#include "../../../../Component/EnemyAttackObject/BossRock/BossRockManager.h"

BossNormalAttack7::BossNormalAttack7()
{
	string = Function::GetClassNameC<BossNormalAttack7>();
	animId = ID::B_N_ATTACK7;
	throwRock = false;
	rockGet = false;
}

BossNormalAttack7::~BossNormalAttack7()
{
}

void BossNormalAttack7::Update()
{
	Boss* boss = GetBase<Boss>();
	EnemyStateBase::Update();
	if (boss->enemyBaseComponent.anim->IsFinish()){
		boss->BossAttackStateChange();
	}
	if (boss->enemyBaseComponent.anim->AnimEventCan()) {
		if (!throwRock) {
			boss->rockManager->ThrowStart();
			throwRock = true;
		}
	}

	AttackSound();
	AttackFlash(ID::B_MODEL, boss->BOSS_RIGHT_HAND_FRAME, "E_AttackV");
	if (boss->enemyBaseComponent.anim->GetCurrentFrame() >= 31.58683f) {
		if (!rockGet) {
			VECTOR3 vzero = VECTOR3(VZero);
			boss->rockManager->CreateThrow(vzero);
			rockGet = true;
		}
	}

	if (boss->enemyBaseComponent.anim->GetCurrentFrame() >= 54.475f && boss->enemyBaseComponent.anim->GetCurrentFrame() <= boss->enemyBaseComponent.anim->EventStartTime(animId)) {
		boss->LookPlayer();
	}
}

void BossNormalAttack7::Draw()
{
}

void BossNormalAttack7::Start()
{
	Boss* boss = GetBase<Boss>();
	EnemyStateBase::Start();
	BossAttackBase::BossStart();
	throwRock = false;
	rockGet = false;
}

void BossNormalAttack7::Finish()
{
	Boss* boss = GetBase<Boss>();
	BossAttackBase::BossFinish();
	boss->enemyBaseComponent.anim->AnimEventReset();
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
}

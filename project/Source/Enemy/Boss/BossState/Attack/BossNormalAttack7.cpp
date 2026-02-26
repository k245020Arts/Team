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
	VECTOR3 vzero = VECTOR3(VZero);
	boss->rockManager->CreateThrow(vzero);
}

void BossNormalAttack7::Finish()
{
}

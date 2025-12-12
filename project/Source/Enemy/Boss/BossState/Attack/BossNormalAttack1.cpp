#include "BossNormalAttack1.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Common/Easing.h"
#include "../BossStatus.h"

BossNormalAttack1::BossNormalAttack1()
{
	//id = ID::B_N_ATTACK1;
	string = Function::GetClassNameC<BossNormalAttack1>();
	animId = ID::B_N_ATTACK1;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	damage.damagePattern = BossAttackBase::NO_BACK;
}

BossNormalAttack1::~BossNormalAttack1()
{
}

void BossNormalAttack1::Update()
{
	EnemyStateBase::Update();
	Boss* boss = GetBase<Boss>();
	if (boss->enemyBaseComponent.anim->GetMaxFrame() - fallFrame <= boss->enemyBaseComponent.anim->GetCurrentFrame())
	{
		boss->BossAttackStateChange();
	}
	BossAttackCollsion();
	if (boss->enemyBaseComponent.anim->AnimEventCan()) {
		averageSpeed = 90.0f / attackTime;
		averageSpeed *= boss->obj->GetObjectRate();
		boss->bossTransform->rotation.y += averageSpeed * DegToRad;
	}
	AttackSound();
	if (boss->maxAttack <= 0) {
		AttackFlash(ID::B_MODEL, boss->BOSS_RIGHT_HAND_FRAME, "E_AttackV");
		damage.flash = true;
	}
	
	BossTrail(true);
	
}

void BossNormalAttack1::Draw()
{
}

void BossNormalAttack1::Start()
{
	Boss* boss = GetBase<Boss>();
	EnemyStateBase::Start();
	BossAttackBase::BossStart();
	firstColl = true;
	boss->enemyBaseComponent.anim->AnimEventReset();
	damage.hitDamage = boss->bs->GetStatus().normalAttack1;
	boss->enemyBaseComponent.anim->SetFrame(5.0f);
	fallFrame = boss->bs->GetStatus().fallFrame;

}

void BossNormalAttack1::Finish()
{
	Boss* boss = GetBase<Boss>();
	boss->DeleteCollision();
	BossAttackBase::BossFinish();
	boss->enemyBaseComponent.anim->AnimEventReset();
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	//boss->threat = true;
}

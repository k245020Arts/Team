#include "BossNormalAttack2.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Common/Easing.h"
#include "../BossStatus.h"

BossNormalAttack2::BossNormalAttack2()
{
	//id = ID::B_N_ATTACK2;
	string					= Function::GetClassNameC<BossNormalAttack2>();
	animId					= ID::B_N_ATTACK2;
	collTrans				= Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	damage.damagePattern	= BossAttackBase::NO_BACK;
	averageSpeed			= 0.0f;
}

BossNormalAttack2::~BossNormalAttack2()
{
}

void BossNormalAttack2::Update()
{
	EnemyStateBase::Update();
	Boss* boss = GetBase<Boss>();
	if (boss->enemyBaseComponent.anim->GetMaxFrame() - fallFrame <= boss->enemyBaseComponent.anim->GetCurrentFrame())
	{
		boss->BossAttackStateChange();
	}
	BossAttackCollsion();
	if (boss->enemyBaseComponent.anim->AnimEventCan()) {
		//UŒ‚‚É‚©‚©‚éŽžŠÔ‚Å90‹‰ñ‚µ‚½‚¢‚Ì‚Å‚PƒtƒŒ[ƒ€‚²‚Æ‚Éi‚ÞŠp“x‚ð‹‚ß‚Ä‚¢‚éB
		averageSpeed = 90.0f / attackTime;
		averageSpeed *= boss->obj->GetObjectTimeRate();

		boss->bossTransform->rotation.y -= averageSpeed * DegToRad;
	}
	AttackSound();
	if (boss->maxAttack <= 0) {
		//ŽO’iUŒ‚‚Ìˆê”ÔÅŒã‚ÌŽž‚¾‚¯Œõ‚éB
		AttackFlash(ID::B_MODEL, boss->BOSS_RIGHT_HAND_FRAME, "E_AttackV");
		damage.flash = true;
	}

	BossTrail(false);
}

void BossNormalAttack2::Draw()
{
}

void BossNormalAttack2::Start()
{
	Boss* boss			= GetBase<Boss>();
	EnemyStateBase::Start();
	BossAttackBase::BossStart();
	firstColl			= true;
	damage.hitDamage	= boss->bs->GetStatus().normalAttack1;

	fallFrame			= boss->bs->GetStatus().fallFrame;
	boss->enemyBaseComponent.anim->SetFrame(5.0f);
	boss->threat		= false;
}

void BossNormalAttack2::Finish()
{
	Boss* boss = GetBase<Boss>();
	boss->DeleteCollision();
	BossAttackBase::BossFinish();
	boss->enemyBaseComponent.anim->AnimEventReset();
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	//boss->threat = true;
}

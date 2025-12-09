#include "BossNormalAttack4.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Common/Easing.h"
#include "../BossStatus.h"
#include "../../../../Component/Physics/Physics.h"

BossNormalAttack4::BossNormalAttack4()
{
	//id = ID::B_N_ATTACK1;
	string = Function::GetClassNameC<BossNormalAttack4>();
	animId = ID::B_N_ATTACK4;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	damage.damagePattern = BossAttackBase::NO_BACK;
}

BossNormalAttack4::~BossNormalAttack4()
{
}

void BossNormalAttack4::Update()
{
	Boss* boss = GetBase<Boss>();
	EnemyStateBase::Update();
	if (boss->enemyBaseComponent.anim->GetMaxFrame() - fallFrame <= boss->enemyBaseComponent.anim->GetCurrentFrame())
	{
		if (boss->maxAttack != -1)
			boss->enemyBaseComponent.state->ChangeState(StateID::ATTACK_SORTING_S);
		else
			boss->enemyBaseComponent.state->ChangeState(StateID::BOSS_RUN_S);
	}
	if (!boss->enemyBaseComponent.anim->AnimEventCan()) {
		VECTOR3 dis = boss->enemyBaseComponent.playerObj->GetTransform()->position - boss->bossTransform->position;
		normal = dis.Normalize();
		boss->enemyBaseComponent.physics->AddVelocity(normal * 6500.0f, true);
	}
	boss->LookPlayer();
	BossAttackCollsion();
	AttackSound();
	if (boss->maxAttack <= 0) {
		AttackFlash(ID::B_MODEL, boss->BOSS_RIGHT_HAND_FRAME, "E_AttackV");
		damage.flash = true;
	}
	BossTrail(true);
}

void BossNormalAttack4::Draw()
{
}

void BossNormalAttack4::Start()
{
	Boss* boss = GetBase<Boss>();
	EnemyStateBase::Start();
	BossAttackBase::BossStart();
	firstColl = true;
	boss->enemyBaseComponent.anim->AnimEventReset();
	keepPlayerPosition = boss->enemyBaseComponent.playerObj->GetTransform()->position;
	damage.hitDamage = boss->bs->GetStatus().normalAttack1;
	VECTOR3 dis = keepPlayerPosition - boss->bossTransform->position;
	normal = dis.Normalize();
	boss->enemyBaseComponent.physics->AddVelocity(normal * 2500.0f, false);

	fallFrame = 0 ;
	boss->threat = false;
	//if (boss->comboFirstAttack)
	//	boss->enemyBaseComponent.anim->SetFrame(0.0f);
	//else
	//	boss->enemyBaseComponent.anim->SetFrame(20.0f);
}

void BossNormalAttack4::Finish()
{
	Boss* boss = GetBase<Boss>();
	boss->DeleteCollision();
	BossAttackBase::BossFinish();
	boss->enemyBaseComponent.anim->AnimEventReset();
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	boss->threat = true;
}

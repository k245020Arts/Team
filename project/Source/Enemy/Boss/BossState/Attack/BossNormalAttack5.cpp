#include "BossNormalAttack5.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Common/Easing.h"
#include "../BossStatus.h"
#include "../../../../Component/Physics/Physics.h"

BossNormalAttack5::BossNormalAttack5()
{
	//id = ID::B_N_ATTACK1;
	string = Function::GetClassNameC<BossNormalAttack5>();
	animId = ID::B_N_ATTACK5;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	damage.damagePattern = BossAttackBase::NO_BACK;
}

BossNormalAttack5::~BossNormalAttack5()
{
}

void BossNormalAttack5::Update()
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
	BossAttackCollsion();
	boss->LookPlayer();
	if (!boss->enemyBaseComponent.anim->AnimEventCan()) {
		VECTOR3 dis = boss->enemyBaseComponent.playerObj->GetTransform()->position - boss->bossTransform->position;
		normal = dis.Normalize();
		boss->enemyBaseComponent.physics->AddVelocity(normal * 6500.0f, true);
	}
	AttackSound();
	AttackFlash(ID::B_MODEL, boss->BOSS_RIGHT_HAND_FRAME, "E_AttackV");
	BossTrail(true);
}

void BossNormalAttack5::Draw()
{
}

void BossNormalAttack5::Start()
{
	Boss* boss = GetBase<Boss>();
	EnemyStateBase::Start();
	firstColl = true;
	boss->enemyBaseComponent.anim->AnimEventReset();
	damage.hitDamage = boss->bs->GetStatus().normalAttack1;
	keepPlayerPosition = boss->enemyBaseComponent.playerObj->GetTransform()->position;
	damage.hitDamage = boss->bs->GetStatus().normalAttack1;
	VECTOR3 dis = keepPlayerPosition - boss->bossTransform->position;
	normal = dis.Normalize();
	boss->enemyBaseComponent.physics->AddVelocity(normal * 2500.0f, false);
	fallFrame = 0;

	/*if (boss->comboFirstAttack)
		boss->enemyBaseComponent.anim->SetFrame(0.0f);
	else
		boss->enemyBaseComponent.anim->SetFrame(10.0f);*/
}

void BossNormalAttack5::Finish()
{
	Boss* boss = GetBase<Boss>();
	boss->DeleteCollision();
	boss->enemyBaseComponent.anim->AnimEventReset();
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
}

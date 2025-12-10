#include "BossNormalAttack6.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Common/Easing.h"
#include "../BossStatus.h"
#include "../../../../Component/Physics/Physics.h"

BossNormalAttack6::BossNormalAttack6()
{
	//id = ID::B_N_ATTACK1;
	string = Function::GetClassNameC<BossNormalAttack6>();
	animId = ID::B_N_ATTACK6;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	damage.damagePattern = BossAttackBase::BACK;
}

BossNormalAttack6::~BossNormalAttack6()
{
}

void BossNormalAttack6::Update()
{
	Boss* boss = GetBase<Boss>();
	EnemyStateBase::Update();
	if (boss->enemyBaseComponent.anim->GetMaxFrame() - fallFrame <= boss->enemyBaseComponent.anim->GetCurrentFrame())
	{
		boss->BossAttackStateChange();
	}
	BossAttackCollsion();
	//boss->LookPlayer();
	if (!boss->enemyBaseComponent.anim->AnimEventCan()) {
		if (firstColl) {
			/*VECTOR3 dis = boss->bossTransform->position - keepPlayerPosition ;
			normal = dis.Normalize();*/
			boss->enemyBaseComponent.physics->AddVelocity(normal * -6500.0f, true);
			
		}
	
	}
	if (boss->enemyBaseComponent.anim->EventFinishTime(animId) - boss->enemyBaseComponent.anim->GetCurrentFrame() <= 5.0f) {
		boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION * 8.0f);
	}
	AttackSound();
	if (boss->maxAttack <= 0) {
		AttackFlash(ID::B_MODEL, boss->BOSS_RIGHT_HAND_FRAME, "E_AttackV");
		damage.flash = true;
	}
	BossTrail(true);
}

void BossNormalAttack6::Draw()
{
}

void BossNormalAttack6::Start()
{
	Boss* boss = GetBase<Boss>();
	EnemyStateBase::Start();
	BossAttackBase::BossStart();
	firstColl = true;
	boss->enemyBaseComponent.anim->AnimEventReset();
	damage.hitDamage = boss->bs->GetStatus().normalAttack1;
	keepPlayerPosition = boss->enemyBaseComponent.playerObj->GetTransform()->position;
	damage.hitDamage = boss->bs->GetStatus().normalAttack1;
	VECTOR3 dis = boss->bossTransform->position - keepPlayerPosition ;
	normal = dis.Normalize();
	boss->bossTransform->rotation.y = atan2f(dis.x, dis.z);
	boss->enemyBaseComponent.physics->AddVelocity(normal * -1500.0f, false);
	fallFrame = 0.0f;
	boss->threat = false;
}

void BossNormalAttack6::Finish()
{
	Boss* boss = GetBase<Boss>();
	boss->DeleteCollision();
	BossAttackBase::BossFinish();
	boss->enemyBaseComponent.anim->AnimEventReset();
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	boss->threat = true;
	boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION);
}

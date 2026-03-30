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
	string					= Function::GetClassNameC<BossNormalAttack5>();
	/*attackParam.animID = ID::B_N_ATTACK5;
	attackParam.attackCollTransform = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	attackParam.damagePattern	= BossAttackBase::NO_BACK;*/
	averageSpeed			= 0.0f;
	keepPlayerPosition		= VZero;
	normal					= VZero;
	oneMove					= false;
	LoadAttackParam();
}

BossNormalAttack5::~BossNormalAttack5()
{
}

void BossNormalAttack5::Update()
{
	Boss* boss = GetBase<Boss>();
	BossAttackBase::Update();
	if (boss->enemyBaseComponent.anim->GetMaxFrame() - fallFrame <= boss->enemyBaseComponent.anim->GetCurrentFrame())
	{
		boss->BossAttackStateChange();
	}
	BossAttackCollsion();
	BossJustAvoidCollsion();
	boss->LookPlayer();
	//攻撃の少し前になったら移動し始める
	if (boss->enemyBaseComponent.anim->EventStartTime(animId) - boss->enemyBaseComponent.anim->GetCurrentFrame() <= 6.0f) {
		//if (oneMove) {
			VECTOR3 dis = boss->enemyBaseComponent.playerObj->GetTransform()->position - boss->bossTransform->position;
			normal = dis.Normalize();
			//y座標をいじりたくないので0にする。
			normal.y = 0.0f;
			if (dis.Size() <= 1000.0f) {
				boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION * 8.0f);
			}
			else {
				float speed = dis.Size();
				speed = std::clamp(speed, 4000.0f, 7000.0f);
				boss->enemyBaseComponent.physics->SetVelocity(normal * speed);
				oneMove = false;
			}
			
		//}
	}
	if (boss->enemyBaseComponent.anim->EventFinishTime(animId) <= boss->enemyBaseComponent.anim->GetCurrentFrame()) {
		boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION * 8.0f);
	}
	AttackSound();
	if (boss->maxAttack <= 0) {
		AttackFlash(ID::B_MODEL, attackParam.attackPositionFrameNum, attackParam.voiceName);
		//damage.flash = true;
	}
	BossTrail(true);
}

void BossNormalAttack5::Draw()
{
}

void BossNormalAttack5::Start()
{
	Boss* boss = GetBase<Boss>();
	//EnemyStateBase::Start();
	BossAttackBase::BossStart();
	firstColl = true;
	boss->enemyBaseComponent.anim->AnimEventReset();
	//attackParam.hitDamage	= boss->bs->GetStatus().normalAttack1;
	keepPlayerPosition	= boss->enemyBaseComponent.playerObj->GetTransform()->position;
	//damage.hitDamage	= boss->bs->GetStatus().normalAttack1;
	/*VECTOR3 dis = keepPlayerPosition - boss->bossTransform->position;
	normal = dis.Normalize();
	boss->enemyBaseComponent.physics->AddVelocity(normal * 2500.0f, false);*/
	fallFrame			= 0;
	boss->threat		= false;
	oneMove				= true;
	/*if (boss->comboFirstAttack)
		boss->enemyBaseComponent.anim->SetFrame(0.0f);
	else
		boss->enemyBaseComponent.anim->SetFrame(10.0f);*/
}

void BossNormalAttack5::Finish()
{
#ifdef DataSave
	DataSaveAll();
#endif // DataSave
	Boss* boss = GetBase<Boss>();
	boss->DeleteCollision(&boss->attackColl);
	BossAttackBase::BossFinish();
	boss->enemyBaseComponent.anim->AnimEventReset();
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION);
	//boss->threat = true;
	oneMove = false;
}

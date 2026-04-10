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
	string					= Function::GetClassNameC<BossNormalAttack6>();
	/*attackParam.animID = ID::B_N_ATTACK6;
	attackParam.attackCollTransform = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	attackParam.damagePattern	= BossAttackBase::BACK;*/
	averageSpeed			= 0.0f;
	keepPlayerPosition		= VZero;
	normal					= VZero;
	LoadAttackParam();
	/*attackParam.playerAloowMove = true;
	attackParam.playerNearStop = true;
	attackParam.maxMoveSpeed = 7000.0f;
	attackParam.minMoveSpeed = 3000.0f;
	attackParam.playerBaseNear = 1000.0f;
	attackParam.lookPlayer = true;
	attackParam.lookNum = 3;
	attackParam.lookMaxCounter = 90.0f;
	attackParam.moveStartTime = 0.0f;
	attackParam.moveFinishTime = 90.0f;
	attackParam.addVelocity = false;

	attackParam.useTrail = true;
	attackParam.trailRightHand = true;*/
}

BossNormalAttack6::~BossNormalAttack6()
{
}

void BossNormalAttack6::Update()
{
	Boss* boss = GetBase<Boss>();
	BossAttackBase::Update();
	//if (boss->enemyBaseComponent.anim->GetMaxFrame() - fallFrame <= boss->enemyBaseComponent.anim->GetCurrentFrame())
	//{
	//	boss->BossAttackStateChange();
	//}
	//BossAttackCollsion();
	//BossJustAvoidCollsion();
	///*if (!boss->enemyBaseComponent.anim->AnimEventCan()) {
	//	if (firstColl) {
	//		for (int i = 0; i < 3; i++) {
	//			boss->LookPlayer();
	//		}
	//		VECTOR3 dis = boss->bossTransform->position - boss->enemyBaseComponent.playerObj->GetTransform()->position ;
	//		normal = dis.Normalize();
	//		if (dis.Size() <= 1000.0f) {
	//			boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION * 8.0f);
	//		}
	//		else {
	//			boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION);
	//			float speed = dis.Size();
	//			if (speed <= 2000.0f) {
	//				speed = 2000.0f;
	//			}
	//			boss->enemyBaseComponent.physics->SetVelocity(normal * -speed);
	//		}
	//		
	//	}
	//
	//}
	//else {
	//	
	//	VECTOR3 dis = boss->bossTransform->position - boss->enemyBaseComponent.playerObj->GetTransform()->position;
	//	if (dis.Size() <= 1000.0f) {
	//		boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION * 8.0f);
	//	}
	//	else {
	//		boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION);
	//		float speed = dis.Size();
	//		if (speed <= 2000.0f) {
	//			speed = 2000.0f;
	//		}
	//		boss->enemyBaseComponent.physics->SetVelocity(normal * -speed);
	//	}
	//	
	//}*/
	//MoveEvent();
	///*if (boss->enemyBaseComponent.anim->EventFinishTime(animId) - boss->enemyBaseComponent.anim->GetCurrentFrame() <= 5.0f) {
	//	boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION * 8.0f);
	//}*/
	//AttackSound();
	//if (boss->maxAttack <= 0) {
	//	AttackFlash(ID::B_MODEL, attackParam.attackPositionFrameNum, attackParam.voiceName);
	//	//damage.flash = true;
	//}
	//BossTrail(true);
}

void BossNormalAttack6::Draw()
{
}

void BossNormalAttack6::Start()
{
	
	Boss* boss = GetBase<Boss>();
	//EnemyStateBase::Start();
	BossAttackBase::BossStart();

	//firstColl = true;
	//boss->enemyBaseComponent.anim->AnimEventReset();

	///*attackParam.hitDamage = boss->bs->GetStatus().normalAttack1;*/
	//keepPlayerPosition	= boss->enemyBaseComponent.playerObj->GetTransform()->position;
	////damage.hitDamage	= boss->bs->GetStatus().normalAttack1;
	//VECTOR3 dis			= boss->bossTransform->position - keepPlayerPosition ;
	//normal				= dis.Normalize();
	////yÀ•W‚ð‚¢‚¶‚è‚½‚­‚È‚¢‚Ì‚Å0‚É‚·‚éB
	//normal.y			= 0.0f;
	//boss->bossTransform->rotation.y = atan2f(dis.x, dis.z);
	//boss->enemyBaseComponent.physics->AddVelocity(normal * -1500.0f, false);
	//
	//fallFrame = 0.0f;
	//boss->threat = false;
}

void BossNormalAttack6::Finish()
{
#ifdef DataSave
	DataSaveAll();
#endif // DataSave
	Boss* boss = GetBase<Boss>();
	BossAttackBase::BossFinish();
	//BossAttackBase::BossFinish();
	//boss->enemyBaseComponent.anim->AnimEventReset();
	//boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	////boss->threat = true;
	//boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION);
}

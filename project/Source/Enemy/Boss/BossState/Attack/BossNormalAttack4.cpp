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
	string					= Function::GetClassNameC<BossNormalAttack4>();
	/*attackParam.animID = ID::B_N_ATTACK4;
	attackParam.attackCollTransform = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	attackParam.damagePattern	= BossAttackBase::NO_BACK;*/
	averageSpeed			= 0.0f;
	keepPlayerPosition		= VZero;
	
	oneMove					= false;
	LoadAttackParam();
	attackParam.playerAloowMove = true;
	attackParam.playerNearStop = true;
	attackParam.maxMoveSpeed = 7000.0f;
	attackParam.minMoveSpeed = 3000.0f;
	attackParam.playerBaseNear = 1000.0f;
	attackParam.baseSpeed = 3000.0f;
	attackParam.lookPlayer = true;
	attackParam.lookNum = 1;
	attackParam.lookMaxCounter = 40.0f;
	attackParam.moveStartTime = 10.0f;
	attackParam.moveFinishTime = 50.0f;
	attackParam.addVelocity = false;

	attackParam.useTrail = true;
	attackParam.trailRightHand = true;
}

BossNormalAttack4::~BossNormalAttack4()
{
}

void BossNormalAttack4::Update()
{
	Boss* boss = GetBase<Boss>();
	BossAttackBase::Update();
	//if (boss->enemyBaseComponent.anim->GetMaxFrame() - fallFrame <= boss->enemyBaseComponent.anim->GetCurrentFrame())
	//{
	//	boss->BossAttackStateChange();
	//}
	//

	////攻撃の少し前になったら移動し始める
	////if (boss->enemyBaseComponent.anim->EventStartTime(attackParam.animID) - boss->enemyBaseComponent.anim->GetCurrentFrame() <= 6.0f){
	////	/*if (oneMove) {
	////		
	////	}*/
	////	VECTOR3 dis = boss->enemyBaseComponent.playerObj->GetTransform()->position - boss->bossTransform->position;
	////	normal = dis.Normalize();
	////	//y座標をいじりたくないので0にする。
	////	normal.y = 0.0f;
	////	if (dis.Size() <= 1000.0f) {
	////		boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION * 8.0f);
	////	}
	////	else {
	////		float speed = dis.Size();
	////		speed = std::clamp(speed, 4000.0f, 7000.0f);
	////		boss->enemyBaseComponent.physics->SetVelocity(normal * speed);
	////		oneMove = false;
	////	}
	////}
	////if (boss->enemyBaseComponent.anim->EventFinishTime(attackParam.animID) <= boss->enemyBaseComponent.anim->GetCurrentFrame()) {
	////	boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION * 8.0f);
	////}
	//MoveEvent();
	////boss->LookPlayer();
	//BossAttackCollsion();
	//BossJustAvoidCollsion();
	//AttackSound();
	//if (boss->maxAttack <= 0) {
	//	AttackFlash(ID::B_MODEL, attackParam.attackPositionFrameNum, attackParam.voiceName);
	//	//attackParam.flash = true;
	//}
	//BossTrail(true);
}

void BossNormalAttack4::Draw()
{
}

void BossNormalAttack4::Start()
{
	Boss* boss = GetBase<Boss>();
	//EnemyStateBase::Start();
	BossAttackBase::BossStart();
	//firstColl = true;
	//boss->enemyBaseComponent.anim->AnimEventReset();

	//keepPlayerPosition	= boss->enemyBaseComponent.playerObj->GetTransform()->position;
	////attackParam.hitDamage	= boss->bs->GetStatus().normalAttack1;
	///*VECTOR3 dis = keepPlayerPosition - boss->bossTransform->position;
	//normal = dis.Normalize();
	//boss->enemyBaseComponent.physics->AddVelocity(normal * 2500.0f, false);*/

	//fallFrame			= 0 ;
	//boss->threat		= false;
	//oneMove				= true;

	//if (boss->comboFirstAttack)
	//	boss->enemyBaseComponent.anim->SetFrame(0.0f);
	//else
	//	boss->enemyBaseComponent.anim->SetFrame(20.0f);
}

void BossNormalAttack4::Finish()
{
#ifdef DataSave
	DataSaveAll();
#endif // DataSave
	Boss* boss = GetBase<Boss>();
	/*boss->DeleteCollision(&boss->attackColl);*/
	BossAttackBase::BossFinish();
	/*boss->enemyBaseComponent.anim->AnimEventReset();
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION);*/
	//boss->threat = true;
}

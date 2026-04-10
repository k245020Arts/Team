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
	//attackParam.animID = ID::B_N_ATTACK7;
	throwRock = false;
	rockGet = false;
	LoadAttackParam();
	attackParam.throwObject = true;
	attackParam.armThrow = true;
	attackParam.armFrameNum = 15;
	attackParam.throwObjectApperaTime = 31.58683f;
	attackParam.throwStartTime = 72.4159164428711f;

	attackParam.lookPlayer = true;
	attackParam.lookMaxCounter = 72.4159164428711f;
	attackParam.lookNum = 1;
	attackParam.throwAttackData.resize(1);
	attackParam.throwAttackData[0].armThrow = true;
	attackParam.throwAttackData[0].armFrameNum = 15;
	attackParam.throwAttackData[0].armAddPos = VZero;
	attackParam.throwAttackData[0].throwToPlayer = true;
	/*attackParam.throwAttackData[0].thorwToFront = true;
	attackParam.throwAttackData[0].throwSpeed = 20000.0f;
	attackParam.throwAttackData[0].throwFirstSpeed = 20000.0f;
	attackParam.throwAttackData[0].diffusionAngle = VECTOR3(0.0f,0.0f, 0.0f);
	attackParam.throwAttackData[0].upSpeed = 0.0f;
	attackParam.throwAttackData[0].throwFallGravity = 0.0f;*/

	
	attackParam.throwAttackData[0].playerAttackObjectDrop = true;
	attackParam.throwAttackData[0].blastBlinkMaxCounter = 0.5f;
	attackParam.throwAttackData[0].blastCan = true;
	attackParam.throwAttackData[0].blastColliderInfo.inRadius = 50.0f;
	attackParam.throwAttackData[0].blastColliderInfo.outRadius = 100.0f;
	attackParam.throwAttackData[0].blastJustAvoidColliderInfo.inRadius = 300;
	attackParam.throwAttackData[0].blastJustAvoidColliderInfo.outRadius = 950.0f;
	attackParam.throwAttackData[0].randCan = true;
	attackParam.throwAttackData[0].randCollInfo.rayStartPos = 300;
	attackParam.throwAttackData[0].randCollInfo.rayFinishPos = -300;

	attackParam.throwAttackData[0].playerHit = true;
	attackParam.throwAttackData[0].playerGroundHit = false;
	attackParam.throwAttackData[0].playerHitCollRadius = 200.0f;
	attackParam.throwAttackData[0].playerHitJustAvoidCollRadius = 300.0f;

	attackParam.throwAttackData[0].playerAttackFlying = true;
	attackParam.throwAttackData[0].playerAttackFlyingCollRadius = 200.0f;

	attackParam.throwAttackData[0].bossHit = true;
	attackParam.throwAttackData[0].bossHitCollRadius = 200.0f;

	attackParam.throwAttackData[0].bossRushHit = true;
	attackParam.throwAttackData[0].bossRushHit = 200.0f;
	attackParam.throwAttackData[0].throwObjectID = "BossAttackRock1";
	attackParam.throwAttackData[0].randTime = 100.0f;
	attackParam.throwAttackData[0].flyingSpeed = 10000.0f;
	attackParam.throwAttackData[0].flyingHeight = 1500.0f;

	attackParam.throwAttackData[0].pushCollCan = true;
}

BossNormalAttack7::~BossNormalAttack7()
{
}

void BossNormalAttack7::Update()
{
	//Boss* boss = GetBase<Boss>();
	BossAttackBase::Update();
	//if (boss->enemyBaseComponent.anim->IsFinish()){
	//	boss->BossAttackStateChange();
	//}
	///*if (boss->enemyBaseComponent.anim->AnimEventCan()) {
	//	if (!throwRock) {
	//		boss->rockManager->ThrowStart();
	//		throwRock = true;
	//	}
	//}*/

	//AttackSound();
	//AttackFlash(ID::B_MODEL, attackParam.attackPositionFrameNum, attackParam.voiceName);
	///*if (boss->enemyBaseComponent.anim->GetCurrentFrame() >= 31.58683f) {
	//	if (!rockGet) {
	//		VECTOR3 vzero = VECTOR3(VZero);
	//		boss->rockManager->CreateThrow(vzero);
	//		rockGet = true;
	//	}
	//}*/

	//ThrowObjectsEvent();
	//LookEvent();

	/*if (boss->enemyBaseComponent.anim->GetCurrentFrame() >= 54.475f && boss->enemyBaseComponent.anim->GetCurrentFrame() <= boss->enemyBaseComponent.anim->EventStartTime(attackParam.animID)) {
		boss->LookPlayer();
	}*/
}

void BossNormalAttack7::Draw()
{
}

void BossNormalAttack7::Start()
{
	Boss* boss = GetBase<Boss>();
	//EnemyStateBase::Start();
	BossAttackBase::BossStart();
	/*throwRock = false;
	rockGet = false;*/
}

void BossNormalAttack7::Finish()
{
#ifdef DataSave
	DataSaveAll();
#endif // DataSave
	Boss* boss = GetBase<Boss>();
	BossAttackBase::BossFinish();
	/*boss->enemyBaseComponent.anim->AnimEventReset();
	boss->enemyBaseComponent.anim->
	SetPlaySpeed(1.0f);*/
}

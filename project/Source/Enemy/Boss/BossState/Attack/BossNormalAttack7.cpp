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

	attackParam.throwAttackData.armThrow = true;
	attackParam.throwAttackData.armFrameNum = 15;
	attackParam.throwAttackData.armAddPos = VZero;
	attackParam.throwAttackData.throwToPlayer = false;
	attackParam.throwAttackData.thorwToFront = true;
	attackParam.throwAttackData.throwSpeed = 20000.0f;
	attackParam.throwAttackData.throwFirstSpeed = 20000.0f;
	attackParam.throwAttackData.diffusionAngle = VECTOR3(0.0f,0.0f, 0.0f);
	attackParam.throwAttackData.upSpeed = 0.0f;
	attackParam.throwAttackData.throwFallGravity = 0.0f;


	attackParam.throwAttackData.playerAttackObjectDrop = true;
	attackParam.throwAttackData.blastBlinkMaxCounter = 0.5f;
	attackParam.throwAttackData.blastCan = true;
	attackParam.throwAttackData.blastColliderInfo.inRadius = 50.0f;
	attackParam.throwAttackData.blastColliderInfo.outRadius = 100.0f;
	attackParam.throwAttackData.blastJustAvoidColliderInfo.inRadius = 300;
	attackParam.throwAttackData.blastJustAvoidColliderInfo.outRadius = 950.0f;
	attackParam.throwAttackData.randCan = true;
	attackParam.throwAttackData.randCollInfo.rayStartPos = 300;
	attackParam.throwAttackData.randCollInfo.rayFinishPos = -300;

	attackParam.throwAttackData.playerHit = true;
	attackParam.throwAttackData.playerGroundHit = false;
	attackParam.throwAttackData.playerHitCollRadius = 200.0f;
	attackParam.throwAttackData.playerHitJustAvoidCollRadius = 300.0f;

	attackParam.throwAttackData.playerAttackFlying = true;
	attackParam.throwAttackData.playerAttackFlyingCollRadius = 200.0f;

	attackParam.throwAttackData.bossHit = true;
	attackParam.throwAttackData.bossHitCollRadius = 200.0f;

	attackParam.throwAttackData.bossRushHit = true;
	attackParam.throwAttackData.bossRushHit = 200.0f;
	attackParam.throwAttackData.throwObjectID = "BossAttackRock1";
	attackParam.throwAttackData.randTime = 100.0f;
	attackParam.throwAttackData.flyingSpeed = 10000.0f;
	attackParam.throwAttackData.flyingHeight = 1500.0f;

	attackParam.throwAttackData.pushCollCan = true;
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

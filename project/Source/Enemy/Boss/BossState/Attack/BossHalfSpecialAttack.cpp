#include "BossHalfSpecialAttack.h"
#include "../../../../Component/EnemyAttackObject/BossRock/BossRock.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../../../Component/Physics/Physics.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Common/Easing.h"
#include "../BossStatus.h"
#include "../../../../Component/EnemyAttackObject/BossRock/BossRockManager.h"

BossHalfSpecialAttack::BossHalfSpecialAttack()
{
    attackParam.animID = ID::B_IDOL;
	string = Function::GetClassNameC<BossHalfSpecialAttack>();
	/*rockCreateCounter = 0.0f;
	oneCreate = false;
    totalRocks = 18;
    rocksDropped = 0;
    interval = 0.3f;
    lastRockCreated = false;
    rockCreateCounter = 10.0f;
    timer = interval;
    rotateAngle = 0.0f;
    rotateSpeed = 1.5f;
    chargingLast = false;
    chargeTimer = 1.2f;*/
    LoadAttackParam();
   /* attackParam.throwObject = true;
    attackParam.armThrow = false;
    attackParam.throwObjectNum = 18;
    attackParam.maxIntervalTime = 0.45f;
    attackParam.minIntervalTime = 0.15f;
    attackParam.intervalTime = 0.3f;
    attackParam.throwAttackData.resize(1);
    attackParam.throwAttackData[0].armThrow = false;
    attackParam.throwAttackData[0].throwToFallToPlayer = true;
    attackParam.throwAttackData[0].randomHeight = true;
    attackParam.throwAttackData[0].minHeight = 9000.0f;
    attackParam.throwAttackData[0].maxHeight = 12000.0f;
    attackParam.throwAttackData[0].randomSpeed = true;
    attackParam.throwAttackData[0].minSpeed = 1500.0f;
    attackParam.throwAttackData[0].maxSpeed = 3000.0f;
    attackParam.throwAttackData[0].randomBlast = true;
    attackParam.throwAttackData[0].randomBlastRate = 0.1f;
    attackParam.throwAttackData[0].playerAttackObjectDrop = false;
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
    
    attackParam.throwAttackData[0].predictionCicleCan = true;
    attackParam.throwAttackData[0].predictionCicleColliderInfo.rayStartPos = 10000;
    attackParam.throwAttackData[0].predictionCicleColliderInfo.rayFinishPos = -10000;

    attackParam.throwAttackData[0].bossHit = true;
    attackParam.throwAttackData[0].bossHitCollRadius = 200.0f;

    attackParam.throwAttackData[0].bossRushHit = true;
    attackParam.throwAttackData[0].bossRushHit = 200.0f;
    attackParam.throwAttackData[0].throwObjectID = "BossAttackRock1";

    attackParam.throwAttackData[0].randTime = 100.0f;
    attackParam.throwAttackData[0].flyingSpeed = 10000.0f;
    attackParam.throwAttackData[0].flyingHeight = 1500.0f;

    attackParam.throwAttackData[0].pushCollCan = true;*/
}

BossHalfSpecialAttack::~BossHalfSpecialAttack()
{
}
void BossHalfSpecialAttack::Update()
{
    Boss* boss = GetBase<Boss>();
    BossAttackBase::Update();

    ThrowObjectsEvent();
}

void BossHalfSpecialAttack::Draw()
{
}

void BossHalfSpecialAttack::Start()
{
	BossAttackBase::BossStart();
	//EnemyStateBase::Start();

    /*totalRocks = 12;
    rocksDropped = 0;

    rotateAngle = 0.0f;
    rotateSpeed = 1.5f;

    chargingLast = false;
    chargeTimer = 1.2f;

    timer = 0.5f;*/
}

void BossHalfSpecialAttack::Finish()
{
#ifdef DataSave
    DataSaveAll();
#endif // DataSave

  
}

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
    attackParam.throwObject = true;
    attackParam.armThrow = false;
    attackParam.throwObjectNum = 18;
    attackParam.maxIntervalTime = 0.45f;
    attackParam.minIntervalTime = 0.15f;
    attackParam.intervalTime = 0.3f;

    attackParam.throwAttackData.armThrow = false;
    attackParam.throwAttackData.throwToFallToPlayer = true;
    attackParam.throwAttackData.randomHeight = true;
    attackParam.throwAttackData.minHeight = 9000.0f;
    attackParam.throwAttackData.maxHeight = 12000.0f;
    attackParam.throwAttackData.randomSpeed = true;
    attackParam.throwAttackData.minSpeed = 1500.0f;
    attackParam.throwAttackData.maxSpeed = 3000.0f;
    attackParam.throwAttackData.randomBlast = true;
    attackParam.throwAttackData.randomBlastRate = 0.1f;
    attackParam.throwAttackData.playerAttackObjectDrop = false;
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
    
    attackParam.throwAttackData.predictionCicleCan = true;
    attackParam.throwAttackData.predictionCicleColliderInfo.rayStartPos = 10000;
    attackParam.throwAttackData.predictionCicleColliderInfo.rayFinishPos = -10000;

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

BossHalfSpecialAttack::~BossHalfSpecialAttack()
{
}
void BossHalfSpecialAttack::Update()
{
    Boss* boss = GetBase<Boss>();

    //rotateAngle += rotateSpeed * Time::DeltaTimeRate();

    //if (!chargingLast)
    //{
    //    timer -= Time::DeltaTimeRate();

    //    if (timer <= 0.0f && rocksDropped < totalRocks)
    //    {
    //        boss->rockManager->CreateRock(rocksDropped,totalRocks,rotateAngle);

    //        rocksDropped++;

    //        // ™X‚É‰Á‘¬
    //        float t = (float)rocksDropped / (float)totalRocks;
    //        timer = Easing::Lerp(0.45f, 0.15f, t);
    //    }

    //    if (rocksDropped >= totalRocks)
    //    {
    //        chargingLast = true;
    //        boss->rockManager->ShakeCamera();
    //    }
    //}
    //else
    //{
    //    chargeTimer -= Time::DeltaTimeRate();

    //    if (chargeTimer <= 0.0f)
    //    {
    //        boss->rockManager->CreateLastRock();
    //        boss->enemyBaseComponent.state->ChangeState(StateID::BOSS_IDOL_S);
    //    }
    //}

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

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
	rockCreateCounter = 0.0f;
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
    chargeTimer = 1.2f;
}

BossHalfSpecialAttack::~BossHalfSpecialAttack()
{
}
void BossHalfSpecialAttack::Update()
{
    Boss* boss = GetBase<Boss>();

    rotateAngle += rotateSpeed * Time::DeltaTimeRate();

    if (!chargingLast)
    {
        timer -= Time::DeltaTimeRate();

        if (timer <= 0.0f && rocksDropped < totalRocks)
        {
            boss->rockManager->CreateRock(rocksDropped,totalRocks,rotateAngle);

            rocksDropped++;

            // ™X‚É‰Á‘¬
            float t = (float)rocksDropped / (float)totalRocks;
            timer = Easing::Lerp(0.45f, 0.15f, t);
        }

        if (rocksDropped >= totalRocks)
        {
            chargingLast = true;
            boss->rockManager->ShakeCamera();
        }
    }
    else
    {
        chargeTimer -= Time::DeltaTimeRate();

        if (chargeTimer <= 0.0f)
        {
            boss->rockManager->CreateLastRock();
            boss->enemyBaseComponent.state->ChangeState(StateID::BOSS_IDOL_S);
        }
    }
}

void BossHalfSpecialAttack::Draw()
{
}

void BossHalfSpecialAttack::Start()
{
	BossAttackBase::BossStart();
	//EnemyStateBase::Start();

    totalRocks = 12;
    rocksDropped = 0;

    rotateAngle = 0.0f;
    rotateSpeed = 1.5f;

    chargingLast = false;
    chargeTimer = 1.2f;

    timer = 0.5f;
}

void BossHalfSpecialAttack::Finish()
{
#ifdef DataSave
    DataSaveAll();
#endif // DataSave

  
}

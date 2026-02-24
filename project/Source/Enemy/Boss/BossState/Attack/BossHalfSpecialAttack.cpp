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
	animId = ID::B_IDOL;
	string = Function::GetClassNameC<BossHalfSpecialAttack>();
	rockCreateCounter = 0.0f;
	oneCreate = false;
    totalRocks = 12;
    rocksDropped = 0;
    interval = 0.3f;
    lastRockCreated = false;
    rockCreateCounter = 10.0f;
    timer = interval;
}

BossHalfSpecialAttack::~BossHalfSpecialAttack()
{
}

void BossHalfSpecialAttack::Update()
{
    Boss* boss = GetBase<Boss>();
    timer -= Time::DeltaTimeRate();

  
    if (timer <= 0.0f && rocksDropped < totalRocks) {
        boss->rockManager->CreateRock(1);
        rocksDropped++;
        timer = interval;
    }

    // 最後の1個（巨大岩）は全て落とした後に生成
    if (rocksDropped >= totalRocks && !lastRockCreated) {
        VECTOR3 playerPos = boss->enemyBaseComponent.playerObj->GetTransform()->position;
        boss->rockManager->CreateLastRock();
        lastRockCreated = true;
    }

    // 攻撃終了判定
    rockCreateCounter -= Time::DeltaTimeRate();
    if (rockCreateCounter <= 0.0f && lastRockCreated) {
        boss->enemyBaseComponent.state->ChangeState(StateID::BOSS_IDOL_S);
    }
}

void BossHalfSpecialAttack::Draw()
{
}

void BossHalfSpecialAttack::Start()
{
	BossAttackBase::Start();
	EnemyStateBase::Start();
    totalRocks = 12;
    rocksDropped = 0;
    interval = 0.3f;
    lastRockCreated = false;
    rockCreateCounter = 10.0f;
    timer = interval;
}

void BossHalfSpecialAttack::Finish()
{
}

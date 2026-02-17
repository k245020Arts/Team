#include "BossHalfSpecialAttack.h"
#include "../../../../Component/EnemyAttackObject/BossRock/BossRock.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../../../Component/Physics/Physics.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Common/Easing.h"
#include "../BossStatus.h"

BossHalfSpecialAttack::BossHalfSpecialAttack()
{
	animId = ID::B_IDOL;
	string = Function::GetClassNameC<BossHalfSpecialAttack>();
	rockCreateCounter = 0.0f;
	oneCreate = false;
}

BossHalfSpecialAttack::~BossHalfSpecialAttack()
{
}

void BossHalfSpecialAttack::Update()
{
	Boss* boss = GetBase<Boss>();
	int rockCounter = (int)rockCreateCounter;
	rockCreateCounter -= Time::DeltaTimeRate();
	if (rockCounter % 2 == 0) {
		
		if (oneCreate) {
			Object3D* rock = new Object3D();
			rock->Init(Transform(VECTOR3(0, 10000, 0), VZero, VOne), "bossRock");
			BossRock* bossRock = rock->Component()->AddComponent<BossRock>();
			Physics* phy = rock->Component()->AddComponent<Physics>();
			phy->Start(VECTOR3(0, -1000, 0), VECTOR3(0, 500, 0));
			bossRock->StartCollAdd(CollsionInformation::BOSS_ROCK, Transform());
			bossRock->SetRockModel();
			oneCreate = false;
		}
		
	}
	else {
		oneCreate = true;
	}
	if (rockCreateCounter <= 0.0f) {
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
	rockCreateCounter = 10.0f;
	oneCreate = true;
}

void BossHalfSpecialAttack::Finish()
{
}

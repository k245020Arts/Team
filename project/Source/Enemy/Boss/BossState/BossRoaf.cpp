#include "BossRoaf.h"
#include "../Boss.h"
#include "../../../State/StateManager.h"
#include "AttackSorting.h"
#include "BossStatus.h"

BossRoaf::BossRoaf()
{
	animId = ID::B_ROAF;
	//id = ID::B_IDOL;
	string = Function::GetClassNameC<BossRoaf>();
}

BossRoaf::~BossRoaf()
{
}

void BossRoaf::Update()
{
	Boss* b = GetBase<Boss>();

}

void BossRoaf::Draw()
{
}

void BossRoaf::Start()
{
	EnemyStateBase::Start();
}

void BossRoaf::Finish()
{
}

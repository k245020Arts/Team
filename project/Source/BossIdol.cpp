#include "BossIdol.h"
#include "Boss.h"
#include "Animator.h"

BossIdol::BossIdol()
{
	animId = ID::B_IDOL;
	id = ID::B_IDOL;
	string = Function::GetClassNameC<BossIdol>();
}

BossIdol::~BossIdol()
{
}

void BossIdol::Update()
{
}

void BossIdol::Draw()
{
}

void BossIdol::Start()
{
	EnemyStateBase::Start();
	//Boss* b = GetBase<Boss>();
	//b->enemyBaseComponent.anim->Play(animId);
}

void BossIdol::Finish()
{
}

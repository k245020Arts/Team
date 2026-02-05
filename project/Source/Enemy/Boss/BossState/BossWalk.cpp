#include "BossWalk.h"
#include "../../../State/StateManager.h"
#include "../Boss.h"

BossWalk::BossWalk()
{
	
}

BossWalk::~BossWalk()
{
}

void BossWalk::Update()
{
	Boss* b = GetBase<Boss>();
	timeCounter += Time::DeltaTimeRate();
	if (timeCounter >= maxCounter)
		b->enemyBaseComponent.state->ChangeState(StateID::ATTACK_SORTING_S);
}

void BossWalk::Start()
{
	timeCounter = 0;
	maxCounter = GetRand(3);
}

void BossWalk::Finish()
{
}

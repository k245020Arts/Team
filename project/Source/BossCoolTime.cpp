#include "BossCoolTime.h"
#include "Boss.h"
#include "stateManager.h"
#include "Animator.h"
#include "BossStatus.h"

BossCoolTime::BossCoolTime()
{
	animId = ID::B_COOLTIME;
	id = ID::B_COOLTIME;
	bs = new BossStatus;
}

BossCoolTime::~BossCoolTime()
{
}

void BossCoolTime::Update()
{
	Boss* b = GetBase<Boss>();
	b->LookPlayer();

	//UŒ‚‚Ü‚Å‚ÌŽžŠÔiŒã‚ÅC³j
	coolTime++;
	if (coolTime >= bs->GetStatus().coolTime)
		b->enemyBaseComponent.state->ChangeState(ID::B_ATTACKSORTING);
}

void BossCoolTime::Start()
{
	coolTime = 0;
	EnemyStateBase::Start();
}

void BossCoolTime::Finish()
{
}

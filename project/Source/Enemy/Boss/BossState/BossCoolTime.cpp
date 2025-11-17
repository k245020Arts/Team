#include "BossCoolTime.h"
#include "../Boss.h"
#include "../../../State/StateManager.h"
#include "../../../Component/Animator/Animator.h"
#include "BossStatus.h"

BossCoolTime::BossCoolTime()
{
	animId = ID::B_COOLTIME;
	id = ID::B_COOLTIME;
}

BossCoolTime::~BossCoolTime()
{
	
}

void BossCoolTime::Update()
{
	Boss* b = GetBase<Boss>();
	b->LookPlayer();
	bs = b->bs;

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

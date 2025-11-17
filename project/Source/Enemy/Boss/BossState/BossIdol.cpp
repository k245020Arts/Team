#include "BossIdol.h"
#include "../Boss.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "BossStatus.h"

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
	Boss* b = GetBase<Boss>();

	VECTOR3 targetVec = b->bossTransform->position - b->enemyBaseComponent.playerObj->GetTransform()->position;

	if (targetVec.Size() < b->bs->GetStatus().chaseRange)
		b->enemyBaseComponent.state->ChangeState(ID::B_RUN);
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

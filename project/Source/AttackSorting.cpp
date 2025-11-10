#include "AttackSorting.h"
#include "Boss.h"
#include "stateManager.h"
#include "BossStatus.h"

AttackSorting::AttackSorting()
{
	coolTime = 0;
}

AttackSorting::~AttackSorting()
{

}

void AttackSorting::Update()
{
	Boss* b = GetBase<Boss>();
	
	if (b->maxAttack == 0)
		b->enemyBaseComponent.state->ChangeState(ID::B_N_ATTACK3);
	else if (b->maxAttack == 1)
		b->enemyBaseComponent.state->ChangeState(ID::B_N_ATTACK1);
	else if (b->maxAttack == 2)
		b->enemyBaseComponent.state->ChangeState(ID::B_N_ATTACK2);
}

void AttackSorting::Start()
{
	Boss* b = GetBase<Boss>();
	BossStatus* bs = b->bs;

	//‰½‰ñ˜A‘±UŒ‚‚·‚é‚©Œˆ‚ß‚é
	if (b->maxAttack == 0)
		b->maxAttack = GetRand(bs->GetStatus().maxAttack);
	else
		b->maxAttack--;
}

void AttackSorting::Finish()
{

}

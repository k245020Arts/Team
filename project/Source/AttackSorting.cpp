#include "AttackSorting.h"
#include "Boss.h"
#include "stateManager.h"

AttackSorting::AttackSorting()
{
	coolTime = 0;
}

AttackSorting::~AttackSorting()
{

}

void AttackSorting::Update()
{
	Boss* b;
	b = GetBase<Boss>();
	int a = GetRand(2);

	if (a == 0)
		b->enemyBaseComponent.state->ChangeState(ID::B_N_ATTACK1);
	else if (a == 1)
		b->enemyBaseComponent.state->ChangeState(ID::B_N_ATTACK2);
	else
		b->enemyBaseComponent.state->ChangeState(ID::B_N_ATTACK3);
}

void AttackSorting::Start()
{
	Boss* b;
	b = GetBase<Boss>();
	//‰½‰ñ˜A‘±UŒ‚‚·‚é‚©Œˆ‚ß‚é
	if (b->maxAttack == 0)
		b->maxAttack = GetRand(MAXATK);
	else
		b->maxAttack--;
}

void AttackSorting::Finish()
{

}

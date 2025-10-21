#include "AttackSorting.h"
#include "Boss.h"
#include "stateManager.h"

AttackSorting::AttackSorting()
{

}

AttackSorting::~AttackSorting()
{

}

void AttackSorting::Update()
{
	int a = GetRand(2);
	Boss* b = GetBase<Boss>();

	if (a == 0)
		b->enemyBaseComponent.state->ChangeState(ID::B_N_ATTACK1);
	else if (a == 1) 
		b->enemyBaseComponent.state->ChangeState(ID::B_N_ATTACK2);
	else 
		b->enemyBaseComponent.state->ChangeState(ID::B_N_ATTACK3);
}

#include "AttackSorting.h"
#include "../Boss.h"
#include "../../State/StateManager.h"
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
	else
	{
		RandomAttack();
		if (!Hp())
			b->maxAttack = 0;
	}

	VECTOR3 targetVec = b->bossTransform->position - b->enemyBaseComponent.playerObj->GetTransform()->position;
	if (targetVec.Size() >= b->bs->GetStatus().range)
	{
		b->enemyBaseComponent.state->ChangeState(ID::B_RUN);
		b->maxAttack = 0;
	}
}

void AttackSorting::Start()
{
	Boss* b = GetBase<Boss>();
	int maxAttack = b->bs->GetStatus().maxAttack;

	if (Hp())
	{
		//‰½‰ñ˜A‘±UŒ‚‚·‚é‚©Œˆ‚ß‚é
		if (b->maxAttack == 0)
			b->maxAttack = GetRand(maxAttack - 1);
		else
			b->maxAttack--;
	}
	else
		b->maxAttack = maxAttack;
}

void AttackSorting::Finish()
{

}

bool AttackSorting::Hp()
{
	Boss* b = GetBase<Boss>();
	float hp = b->hp;

	//hp‚ª‚WŠ„ˆÈ‰º‚È‚ç˜A‘±UŒ‚‚ğ‚·‚é
	if (hp <= b->bs->GetStatus().maxHp * 0.8)
		return true;
	else
		return false;
}

void AttackSorting::RandomAttack()
{
	Boss* b = GetBase<Boss>();
	int random = GetRand(1);
	if (random == 0)
		b->enemyBaseComponent.state->ChangeState(ID::B_N_ATTACK1);
	else if (random == 1)
		b->enemyBaseComponent.state->ChangeState(ID::B_N_ATTACK2);
}

#include "AttackSorting.h"
#include "../Boss.h"
#include "../../../State/StateManager.h"
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
	if (jump) {
		if (b->maxAttack != 0) {
			b->enemyBaseComponent.state->ChangeState(StateID::BOSS_SPECIAL_SMALL_ATTACK1_S);
			b->maxAttack--;
		}
		else {
			b->enemyBaseComponent.state->ChangeState(StateID::BOSS_SPECIAL_ATTACK1_S);
		}
			
	}
	else {
		if (b->maxAttack == 0)
			b->enemyBaseComponent.state->ChangeState(StateID::BOSS_SPECIAL_ATTACK1_S);
		else if (b->maxAttack == 1)
			b->enemyBaseComponent.state->ChangeState(StateID::BOSS_NORMAL_ATTACK1_S);
		else if (b->maxAttack == 2)
			b->enemyBaseComponent.state->ChangeState(StateID::BOSS_NORMAL_ATTACK2_S);
		else if (b->maxAttack == 3)
			b->enemyBaseComponent.state->ChangeState(StateID::BOSS_NORMAL_ATTACK3_S);
		else
		{
			RandomAttack();
			if (!Hp())
				b->maxAttack = 0;
		}
	}

	

	/*VECTOR3 targetVec = b->bossTransform->position - b->enemyBaseComponent.playerObj->GetTransform()->position;
	if (targetVec.Size() >= b->bs->GetStatus().range)
	{
		b->enemyBaseComponent.state->ChangeState(ID::B_RUN);
		b->maxAttack = 0;
	}*/
}

void AttackSorting::Start()
{
	Boss* b = GetBase<Boss>();
	int maxAttack = b->bs->GetStatus().maxAttack;
	int randam = GetRand(1);
	if (Hp())
	{
		//âΩâÒòAë±çUåÇÇ∑ÇÈÇ©åàÇﬂÇÈ
		if (b->maxAttack == 0)
			b->maxAttack = GetRand(maxAttack - 1);
		else
			b->maxAttack--;
	}
	else
		b->maxAttack = 0;

	if (b->maxAttack == 0) {
		if (randam) {
			jump = true;
			b->maxAttack = GetRand(maxAttack - 1);
		}
		else {
			jump = false;
		}
	}
	
}

void AttackSorting::Finish()
{

}

bool AttackSorting::Hp()
{
	Boss* b = GetBase<Boss>();
	float hp = b->hp;

	//hpÇ™ÇWäÑà»â∫Ç»ÇÁòAë±çUåÇÇÇ∑ÇÈ
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
		b->enemyBaseComponent.state->ChangeState(StateID::BOSS_NORMAL_ATTACK1_S);
	else if (random == 1)
		b->enemyBaseComponent.state->ChangeState(StateID::BOSS_NORMAL_ATTACK2_S);
}

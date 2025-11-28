#include "AttackSorting.h"
#include "../Boss.h"
#include "../../../State/StateManager.h"
#include "BossStatus.h"
#include "../../../Common/Random.h"

namespace {
	const int ATTACK_KIND_MAX = 6;
	const int COMBO_ATTACK_KIND_MAX = 3;
	enum COMBO_ATTACK
	{
		NORMAL_COMBO,
		JUMP_COMBO,
		RUN_COMBO,
	};
	std::vector<COMBO_ATTACK> comboKind{
		{NORMAL_COMBO},
		{JUMP_COMBO},
		{RUN_COMBO},
	};
	std::vector<StateID::State_ID> attackKind{
		{StateID::BOSS_NORMAL_ATTACK1_S},
		{StateID::BOSS_NORMAL_ATTACK2_S},
		{StateID::BOSS_NORMAL_ATTACK3_S},
		{StateID::BOSS_SPECIAL_ATTACK1_S},
		{StateID::BOSS_SPECIAL_SMALL_ATTACK1_S},
		{StateID::BOSS_SPECIAL_ATTACK2_S},
	};
	const std::vector<std::vector<double>> normalAttackParam{
		{ 1.0,1.0,0.5,0.0,0.0,0.0 },
		{ 1.0,1.0,1.0,0.0,0.0,0.0 },
		{ 0.1,0.1,0.5,0.7,1.0,0.1 },
		{ 0.05,0.05,1.0,0.5,0.9,0.1 },
	};
	const std::vector<std::vector<double>> comboAttackParam{
		{ 1.0,0.0,0.0},
		{ 0.8,0.2,0.0},
		{ 0.5,0.5,0.5},
		{ 0.2,0.7,0.5},
	};
	const std::vector<std::vector<StateID::State_ID>> comboOrder{
		{StateID::BOSS_NORMAL_ATTACK1_S, StateID::BOSS_NORMAL_ATTACK2_S, StateID::BOSS_NORMAL_ATTACK3_S},
		{StateID::BOSS_SPECIAL_SMALL_ATTACK1_S, StateID::BOSS_SPECIAL_SMALL_ATTACK1_S, StateID::BOSS_SPECIAL_ATTACK1_S },
		{StateID::BOSS_SPECIAL_ATTACK2_S, StateID::BOSS_SPECIAL_ATTACK2_S }
	};
}

AttackSorting::AttackSorting()
{
	coolTime = 0;
	string = Function::GetClassNameC<AttackSorting>();
	hp = Boss::MAX;
	attackNum = 0;
	jump = false;
	kind = 0;

}

AttackSorting::~AttackSorting()
{

}

void AttackSorting::Update()
{
	Boss* b = GetBase<Boss>();
	if (b->maxAttack != -1) {
		b->enemyBaseComponent.state->ChangeState(comboOrder[kind][attackNum - b->maxAttack]);
	}
	else {
		b->enemyBaseComponent.state->ChangeState(attackKind[kind]);
	}
	
	/*Boss* b = GetBase<Boss>();
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
	}*/

	

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
	if (b->maxAttack != -1) {
		b->maxAttack--;
		b->comboFirstAttack = false;
		return;
	}
	int maxAttack = b->bs->GetStatus().maxAttack;
	int randam = GetRand(1);
	hp = b->Hp();
	float comboAttackRate = 0.0f;
	switch (hp)
	{
	case Boss::MAX:
		comboAttackRate = 0.0f;
		break;
	case Boss::EIGHT:
		comboAttackRate = 0.2f;
		break;
	case Boss::FIVE:
		comboAttackRate = 0.7f;
		break;
	case Boss::THREE:
		comboAttackRate = 0.9f;
		break;
	}
	bool combo = Random::GetBernoulli(comboAttackRate);
	if (combo) {
		std::vector<double> rand = comboAttackParam[hp];
		VECTOR3 dist = b->obj->GetTransform()->position - b->enemyBaseComponent.playerObj->GetTransform()->position;
		float size = dist.Size();
		if (size >= 5000.0f) {
			rand[2] += 2.0f;
		}
		kind = Random::GetWeightedIndex(rand);
		b->maxAttack = comboOrder[kind].size() - 1;
		attackNum = b->maxAttack;
		b->comboFirstAttack = true;
	}
	else {
		NormalAttackSelect();
	}
	//if ()
	//{
	//	//‰½‰ñ˜A‘±UŒ‚‚·‚é‚©Œˆ‚ß‚é
	//	if (b->maxAttack == 0)
	//		b->maxAttack = GetRand(maxAttack - 1);
	//	else
	//		b->maxAttack--;
	//}
	//else
	//	b->maxAttack = 0;

	//if (b->maxAttack == 0) {
	//	if (randam) {
	//		jump = true;
	//		b->maxAttack = GetRand(maxAttack + 10);
	//	}
	//	else {
	//		jump = false;
	//	}
	//}
	
}

void AttackSorting::Finish()
{

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

void AttackSorting::NormalAttackSelect()
{
	Boss* b = GetBase<Boss>();
	std::vector<double> rand = normalAttackParam[hp];
	/*switch (hp)
	{
	case AttackSorting::MAX:
		rand = 
		break;
		break;
	case AttackSorting::EIGHT:
		rand = 
		break;
	case AttackSorting::FIVE:
		
		break;
	case AttackSorting::THREE:
		
		break;
	default:
		break;
	}*/
	VECTOR3 dist = b->obj->GetTransform()->position - b->enemyBaseComponent.playerObj->GetTransform()->position;
	float size = dist.Size();
	if (size > 5000.0f) {
		rand[5] += 3.0f;
	}
	kind = Random::GetWeightedIndex(rand);
	b->maxAttack = -1;
}
